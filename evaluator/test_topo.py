import sys

from numpy import sort
from rich import traceback

from .topological import Gate, QFTTopo

traceback.install()


if __name__ == "__main__":
    num = int(sys.argv[1])
    assert num >= 0, num
    qft_topo: QFTTopo = QFTTopo(num)
    for gate in qft_topo.gates:
        print(gate)
