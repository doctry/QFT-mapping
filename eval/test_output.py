import json
import sys

from numpy import sort
from rich import traceback
from test_device import Device, Operation, Qubit
from test_topo import Gate, QFTTopo

traceback.install()


class Checker:
    def __init__(
        self,
        device: Device,
        qft_topo: QFTTopo,
        operations: list[Operation],
        cycle: dict,
    ):
        self.device: Device = device
        self.qft_topo: QFTTopo = qft_topo
        self.operations: list[Operation] = operations
        self.cycle: dict = cycle

    def apply_gate(self, op: Operation, d_q0: Qubit, d_q1: Qubit) -> None:
        (start, end) = op.duration
        assert start >= d_q0.occupied_until and start >= d_q1.occupied_until, (
            start,
            d_q0.occupied_until,
            d_q1.occupied_until,
        )

        assert end == start + self.cycle[op.operator], (end, start, op.operator)
        d_q0.occupied_until = end
        d_q1.occupied_until = end

    def apply_Swap(self, op: Operation) ->None:
        assert op.operator == "Swap", op.operator
        (d_q0_idx, d_q1_idx) = op.qubits
        d_q0 = self.device.qubits[d_q0_idx]
        d_q1 = self.device.qubits[d_q1_idx]
        self.apply_gate(op, d_q0, d_q1)
        d_q0.topo, d_q1.topo = d_q1.topo, d_q0.topo

    def apply_R(self, op: Operation, gate: Gate) -> bool:
        assert op.operator == "R", op.operator
        (d_q0_idx, d_q1_idx) = op.qubits
        d_q0 = self.device.qubits[d_q0_idx]
        d_q1 = self.device.qubits[d_q1_idx]
        topos: list[int] = [d_q0.topo, d_q1.topo]
        topos.sort()

        if topos != list(gate.qubits):
            return False

        self.apply_gate(op, d_q0, d_q1)

        return True

    def test_operations(self) -> int:
        finished_gates: list[int] = []
        for op in self.operations:
            if op.operator == "Swap":
                self.apply_Swap(op)
            elif op.operator == "R":
                avail_gates = self.qft_topo.avail_gates
                pass_condition = False
                for g in avail_gates:
                    if self.apply_R(op, self.qft_topo.gates[g]):
                        pass_condition = True
                        qft_topo.update_avail_gates(g)
                        finished_gates.append(g)
                        break
                if pass_condition == False:
                    print("Executed gates:")
                    for g in finished_gates:
                        print(self.qft_topo.gates[g])
                    print("Available gates:")
                    for g in avail_gates:
                        print(self.qft_topo.gates[g])
                    print("Device status:")
                    print(self.device)
                    print("Failed Operation", op)
                    raise RuntimeError("Operation cannot match device.")

        return self.operations[-1].duration[1]


if __name__ == "__main__":
    num = int(sys.argv[1])
    assert num > 0
    qft_topo = QFTTopo(num)

    device_filename = sys.argv[2]
    device = Device(device_filename)

    output_filename = sys.argv[3]
    output_file = open(output_filename, "r")
    output = json.load(output_file)

    R_cycle = int(sys.argv[4])
    Swap_cycle = int(sys.argv[5])

    cycle: dict[int] = {"R": R_cycle, "Swap": Swap_cycle}

    operations = output["Operations"]
    final_cost: int = output["final_cost"]

    operations: list[Operation] = [
        Operation(o["Operator"], tuple(o["Qubits"]), tuple(o["duration"]))
        for o in operations
    ]

    checker = Checker(device, qft_topo, operations, cycle)

    test_cost = checker.test_operations()

    assert test_cost == final_cost, (test_cost, final_cost)
