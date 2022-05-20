from .topo import Gate, Topo


def make_dict(next: int):
    return {"id": next, "done": False}


class QFTTopo(Topo):
    def __init__(self, num: int):
        super().__init__()
        assert num > 0, num

        self.gates: list[Gate] = []

        count = 0
        for i in range(num):
            for j in range(i):
                next_0 = -1 if i == num - 1 else (count + i)
                next_1 = -1 if j == i - 1 else (count + 1)
                prev_0 = -1 if j == i - 1 else (count + 1 - i)
                prev_1 = -1 if j == 0 else (count - 1)
                gate = Gate(
                    count,
                    "R",
                    [j, i],
                    [make_dict(prev_0), make_dict(prev_1)],
                )
                gate.add_next(next_0)
                gate.add_next(next_1)
                self.gates.append(gate)
                count += 1

        self.avail_gates: list[int] = [0]

    def update_avail_gates(self, executed: int) -> None:
        assert executed in self.avail_gates
        g_exec = self.gates[executed]
        self.avail_gates.remove(executed)

        assert g_exec.id == executed, (g_exec.id, executed)

        nexts = g_exec.next

        for n in nexts:
            next = self.gates[n]
            next.finished(executed)
            if next.is_avail():
                self.avail_gates.append(n)
