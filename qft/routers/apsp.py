from typing import List

from networkx import Graph
from scipy.sparse import csgraph

from qft.devs import Device
import networkx as nx
from .interfaces import Router


class APSPRouter(Router):
    def __init__(self, device: Device) -> None:
        self.device = device
        self.distances = nx.shortest_path(self.dev_graph)

    @property
    def dev_graph(self) -> Graph:
        return self.device.g

    def route_physical(self, source: int, target: int) -> List[int]:
        return self.distances[source][target]
