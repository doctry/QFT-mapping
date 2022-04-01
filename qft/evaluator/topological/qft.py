from .gates import Gate


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
                gate = Gate(count, (j, i), [next_0, next_1], [prev_0, prev_1])
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
            if next.is_avail(executed):
                self.avail_gates.append(n)
