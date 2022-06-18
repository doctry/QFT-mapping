from __future__ import annotations

import json
import sys
from typing import Dict, List

from rich import traceback

from qft.evaluator.physical import Device, Operation, Qubit
from qft.evaluator.topological import AlgoTopo, Gate, QFTTopo, Topo

traceback.install()


class Checker:
    def __init__(
        self,
        device: Device,
        topo: Topo,
        operations: List[Operation],
        cycle: dict,
    ):
        self.device: Device = device
        self.topo: Topo = topo
        self.operations: List[Operation] = operations
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

    def apply_Swap(self, op: Operation) -> None:
        assert op.operator == "Swap", op.operator
        (d_q0_idx, d_q1_idx) = op.qubits
        d_q0 = self.device.qubits[d_q0_idx]
        d_q1 = self.device.qubits[d_q1_idx]
        self.apply_gate(op, d_q0, d_q1)
        d_q0.topo, d_q1.topo = d_q1.topo, d_q0.topo

    def apply(self, op: Operation, gate: Gate) -> bool:
        assert op.operator == "R" or op.operator == "CX", op.operator
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
            elif op.operator == "CX":
                avail_gates = self.topo.avail_gates
                pass_condition = False
                for g in avail_gates:
                    if self.apply(op, self.topo.gates[g]):
                        pass_condition = True
                        topo.update_avail_gates(g)
                        finished_gates.append(g)
                        break
                if pass_condition == False:
                    print("Executed gates:")
                    for g in finished_gates:
                        print(self.topo.gates[g])
                    print("Available gates:")
                    for g in avail_gates:
                        print(self.topo.gates[g])
                    print("Device status:")
                    print(self.device)
                    print("Failed Operation", op)
                    raise RuntimeError("Operation cannot match device.")

        print("num gates:", len(finished_gates))
        print("num operations:", len(self.operations))
        assert len(finished_gates) == len(self.topo.gates), [
            len(finished_gates),
            len(self.topo.gates),
        ]
        return self.operations[-1].duration[1]


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        cfg = json.load(f)

    # init topo
    algo: int | str = cfg["algo"]
    if isinstance(algo, int):
        assert algo > 0
        topo = QFTTopo(algo)
    elif isinstance(algo, str):
        topo = AlgoTopo(algo)

    # init device
    device_filename = cfg["device"]
    device = Device(device_filename)

    output_filename = cfg["output"]
    with open(output_filename, "r") as o:
        output = json.load(o)

    # initial place
    assign: List[int] = output["initial"]
    device.place(assign)

    cycle: Dict[str, int] = cfg["cycle"]

    ops = output["Operations"]
    final_cost: int = output["final_cost"]

    operations: list[Operation] = [
        Operation(o["Operator"], tuple(o["Qubits"]), tuple(o["duration"])) for o in ops  # type: ignore
    ]
    checker = Checker(device, topo, operations, cycle)

    test_cost = checker.test_operations()

    assert test_cost == final_cost, (test_cost, final_cost)

    print("pass test")
