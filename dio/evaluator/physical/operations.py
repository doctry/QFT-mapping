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
