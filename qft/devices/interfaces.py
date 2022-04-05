from typing import Protocol

from networkx import Graph


class Device(Protocol):
    graph: Graph
