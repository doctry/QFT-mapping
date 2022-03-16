import sys

from rich import traceback
from test_device import Device, Operation
from test_topo import QFTTopo

traceback.install()


def test_operations(operations: list[Operation], qft_topo: QFTTopo, device: Device):
    # for op in operations:

    return operations[-1].duration[1]


if __name__ == "__main__":
    num = int(sys.argv[1])
    assert num > 0
    qft_topo = QFTTopo(num)

    device_filename = sys.argv[2]
    device = Device(device_filename)

    output_filename = sys.argv[3]
    output_file = open(output_filename, "r")
    output = json.load(output_file)

    operations = output["Operations"]
    final_cost = output["final_cost"]

    operations = [
        Operation(o["Operator"], tuple(o["Qubits"]), tuple(o["duration"]))
        for o in operations
    ]

    test_cost = test_operations(operations, qft_topo, device)

    assert test_cost == final_cost, (test_cost, final_cost)
