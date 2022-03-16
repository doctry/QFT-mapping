import sys
from rich import traceback

traceback.install()



class Gate:
    def __init__(
        self,
        id: int,
        qubits: tuple[int, int],
        next: list[int or None, int or None],
        prev: list[int or None, int or None],
    ):
        assert id > -1
        self.id = id

        assert qubits[0] > -1 and qubits[1] > -1
        self.qubits = qubits

        self.next = [n for n in next if n is not None]

        self.prev = [p for p in prev if p is not None]
        self.prev_mark = [False for _ in self.prev]

    def __str__(self):
        return "Gate{}: (Q{}, Q{}), next:{}, prev:{}".format(
            self.id, self.qubits[0], self.qubits[1], self.next, self.prev
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

        self.avail_gates: list[int] = []

    def update_avail_gates(self, executed: int):
        assert executed in self.avail_gates
        g_exec = self.gates[executed]
        self.avail_gates.remove(executed)

        assert g_exec.id == executed, (g_exec.id, executed)

        nexts = g_exec.next

        for n in nexts:
            next = self.gates[n]
            assert executed in next.prev
            assert next.prev_mark[next.prev.index(executed)] == False, next.prev.index(
                executed
            )
            next.prev_mark[next.prev.index(executed)] = True
            if all(m == True for m in next.prev_mark):
                self.avail_gates.append(next)


if __name__ == "__main__":
    num = int(sys.argv[1])
    assert num >= 0, num
    qft_topo: QFTTopo = QFTTopo(num)
    for gate in qft_topo.gates:
        print(gate)
