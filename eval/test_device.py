import sys

from numpy import sort


class Operation:
    def __init__(
        self, operator: str, qubits: tuple[int, int], duration: tuple[int, int]
    ):
        assert operator == "R" or operator == "Swap", operator
        self.operator = operator

        assert qubits[0] > -1 and qubits[1] > -1
        self.qubits = qubits
        self.qubits = sorted(self.qubits)

        assert duration[0] > -1 and duration[1] > -1
        self.duration = duration

    def __str__(self):
        return (
            self.operator
            + " (Q"
            + str(self.qubits[0])
            + " ,Q"
            + str(self.qubits[1])
            + ") duration:"
            + str(self.duration)
        )


class Qubit:
    def __init__(self, id: int, adj_list: list[int]):
        assert id > -1, id
        self.id = id
        self.adj_list = adj_list
        self.occupied_until: int = 0
        self.topo = id

        for i in self.adj_list:
            assert i > -1, self.adj_list

    def is_adj(self, other: int) -> bool:
        assert other > -1
        return other in self.adj_list

    def no_conflict(self, time: int) -> bool:
        return self.occupied_until < time

    def to_json(self) -> dict:
        return {"id": self.id, "adj_list": self.adj_list}

    def __str__(self) :
        return "id: {}, topo_qubit: {}, occupied_until: {}".format(self.id, self.topo, self.occupied_until)


class Device:
    def __init__(self, filename: str):
        nums = []
        self.qubits: list[Qubit] = []

        with open(filename, "r") as f:
            for x in f:
                x = x.split()
                for y in x:
                    nums.append(int(y))

        count = 0
        num = nums[0]
        for i in range(num):
            count += 1
            id = nums[count]
            assert i == id, (i, id)

            count += 1
            num_adj = nums[count]
            adj = []
            for _ in range(num_adj):
                count += 1
                adj.append(nums[count])
            self.qubits.append(Qubit(i, adj))

        assert len(self.qubits) == num

        for (i, qubit) in enumerate(self.qubits):
            for j in qubit.adj_list:
                assert i in self.qubits[j].adj_list, (i, j, self.qubits[j].adj_list)

    def to_json(self) -> list[dict]:
        ret = []
        for q in self.qubits:
            ret.append(q.to_json())
        return ret

    def __str__(self):
        ret = ""
        for q in self.qubits:
            ret += q.__str__()+"\n"
        return ret

if __name__ == "__main__":
    filename = sys.argv[1]
    device = Device(filename)
