import sys


class Gate:
    def __init__(
        self,
        id: int,
        qubits: tuple[int, int],
        next: tuple[int, int],
        prev: tuple[int, int],
    ):
        assert id > -1
        self.id = id

        assert qubits[0] > -1 and qubits[1] > -1
        self.qubits = qubits

        self.next = next

        self.prev = {}
        if prev[0] is not None:
            self.prev[prev[0]] = False
        if prev[1] is not None:
            self.prev[prev[1]] = False

    def __str__(self):
        prev = []
        for n in self.prev:
            prev.append(n)
        prev_0 = None if len(prev) < 1 else prev[0]
        prev_1 = None if len(prev) < 2 else prev[1]
        return "Gate{}: (Q{}, Q{}), next:({}, {}), prev:({}, {})".format(
            self.id,
            self.qubits[0],
            self.qubits[1],
            self.next[0],
            self.next[1],
            prev_0,
            prev_1,
        )

    def is_avail(self, gate_id: int):
        assert gate_id > -1
        assert self.prev[gate_id] is not None

        ret = True
        self.prev[gate_id] = True
        for done in self.prev:
            ret = ret and self.prev[done]

        return ret


class QFTTopo:
    def __init__(self, num: int):
        assert num > 0, num

        self.gates: list[Gate] = []

        count = 0
        for i in range(num):
            for j in range(i):
                next_0 = None if i == num - 1 else (count + i)
                next_1 = None if j == i - 1 else (count + 1)
                prev_0 = None if j == i - 1 else (count + 1 - i)
                prev_1 = None if j == 0 else (count - 1)
                gate = Gate(count, (j, i), (next_0, next_1), (prev_0, prev_1))
                self.gates.append(gate)
                count += 1


if __name__ == "__main__":
    num = int(sys.argv[1])
    assert num >= 0, num
    qft_topo: QFTTopo = QFTTopo(num)
    for gate in qft_topo.gates:
        print(gate)
