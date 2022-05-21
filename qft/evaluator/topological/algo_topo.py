from .topo import Gate, Topo


class AlgoTopo(Topo):
    def __init__(self, file: str):
        self.qubit2gate = dict()
        self.gates = list()
        self.avail_gates = list()
        self.parseQasm(file)

    def update_avail_gates(self, executed: int) -> None:
        self.avail_gates.remove(executed)
        nexts = self.gates[executed].get_next()
        for item in nexts:
            self.gates[item].finished(executed)

            if self.gates[item].is_avail():
                self.avail_gates.append(item)

    def parseQasm(self, file):
        lastCnotWith = list()
        with open(file, "r") as f:
            for i in range(3):
                line: str = f.readline()
            qubit_num = int(line.split()[1].split("[")[1].split("]")[0])  # type: ignore
            for i in range(qubit_num):
                self.qubit2gate[i] = -1
                lastCnotWith.append({"id": -1, "count": 1})
            id = 0
            while line != "":  # type: ignore
                line = f.readline()
                li = line.split(" ")
                gtype = li[0]
                if gtype == "cx":
                    [q1, q2] = li[1].split(",")
                    q1 = int(q1[2:-1])
                    q2 = int(q2[2:-3])
                    if (lastCnotWith[q1]["id"] == q2) and (lastCnotWith[q2]["id"] == q1):
                        assert lastCnotWith[q1]["count"] < 2 and lastCnotWith[q2]["count"] < 2

                        lastCnotWith[q1]["count"] += 1
                        lastCnotWith[q2]["count"] += 1
                        self.gates[id-1].typ = "R"
                    else:
                        lastCnotWith[q1]["id"] = q2
                        lastCnotWith[q2]["id"] = q1
                        lastCnotWith[q1]["count"] = 1
                        lastCnotWith[q2]["count"] = 1
                        
                        temp = Gate(
                            id,
                            "Cx",
                            [q1, q2],
                            [
                                {"id": self.qubit2gate[q1], "done": False},
                                {"id": self.qubit2gate[q2], "done": False},
                            ],
                        )
                        if self.qubit2gate[q1] != -1:
                            self.gates[self.qubit2gate[q1]].add_next(id)
                        if self.qubit2gate[q1] != self.qubit2gate[q2]:
                            if self.qubit2gate[q2] != -1:
                                self.gates[self.qubit2gate[q2]].add_next(id)

                        self.qubit2gate[q1] = id
                        self.qubit2gate[q2] = id
                        self.gates.append(temp)
                        id += 1

        for gate in self.gates:
            if gate.is_avail():
                self.avail_gates.append(gate.id)
