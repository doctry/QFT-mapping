from typing import List

import networkx as nx
from networkx import Graph

from qft.devs import Device

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
