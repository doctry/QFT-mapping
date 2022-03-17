import json
import sys

from numpy import sort
from rich import traceback
from test_device import Device, Operation
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

    def apply_gate(self, op: Operation, gate: Gate) -> bool:
        (d_q0_idx, d_q1_idx) = op.qubits
        d_q0 = self.device.qubits[d_q0_idx]
        d_q1 = self.device.qubits[d_q1_idx]
        topos: list[int] = [d_q0.topo, d_q1.topo]
        sort(topos)
        if topos != gate.qubits:
            return False

        (start, end) = op.duration
        assert start >= d_q0.occupied_until and start >= d_q1.occupied_until, (
            start,
            d_q0.occupied_until,
            d_q1.occupied_until,
        )
        assert end == start + self.cycle[op.operator], (end, start, op.operator)
        d_q0.occupied_until = end
        d_q1.occupied_until = end

        if op.operator == "Swap":
            d_q0.topo, d_q1.topo = d_q1.topo, d_q0.topo

        return True

    def test_operations(self) -> int:
        for op in self.operations:
            avail_gates = self.qft_topo.avail_gates
            pass_condition = False
            for g in avail_gates:
                if self.apply_gate(op, self.qft_topo.gates[g]):
                    pass_condition = True
                    qft_topo.update_avail_gates(g)
            assert pass_condition

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
