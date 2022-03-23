class Gate:
    def __init__(
        self,
        id: int,
        qubits: tuple[int, int],
        next: list[int or None],
        prev: list[int or None],
    ):
        assert id > -1
        self.id = id

        assert qubits[0] > -1 and qubits[1] > -1
        self.qubits = qubits
        sort(self.qubits)

        self.next = [n for n in next if n is not None]

        self.prev = [p for p in prev if p is not None]
        self.prev_mark = [False for _ in self.prev]

    def __str__(self):
        return "Gate{}: (Q{}, Q{}), next:{}, prev:{}".format(
            self.id, self.qubits[0], self.qubits[1], self.next, self.prev
        )

    def is_avail(self, gate_id: int) -> bool:
        assert gate_id > -1
        assert gate_id in self.prev
        assert self.prev_mark[self.prev.index(gate_id)] == False, self.prev.index(
            gate_id
        )
        self.prev_mark[self.prev.index(gate_id)] = True

        return all(m == True for m in self.prev_mark)
