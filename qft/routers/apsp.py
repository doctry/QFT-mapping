from typing import Dict, List, Tuple

import networkx as nx
from networkx import Graph

from qft.devs import Device

from .interfaces import Router


class APSPRouter(Router):
    def __init__(self, device: Device) -> None:
        self.device = device
        self.shortest_paths = self._flattened_shortest_paths()

    @property
    def dev_graph(self) -> Graph:
        return self.device.g

    def route_physical(self, source: int, target: int) -> Tuple[List[int], List[int]]:
        path = self.shortest_paths[source, target]
        length = len(path)
        return (
            self.shortest_paths[: length // 2 : 1],
            self.shortest_paths[: length // 2 - 1 : -1],
        )

    def _flattened_shortest_paths(self) -> Dict[Tuple[int, int], List[int]]:
        result = {}

        shortest_paths = nx.shortest_path(self.dev_graph)
        for (source, target_paths) in shortest_paths.items():
            for (target, paths) in target_paths.items():
                result[source, target] = paths
                assert paths[0] == source, [paths, source]
                assert paths[-1] == target, [paths, target]

        for (source, target) in result.keys():
            assert result[source, target] == result[target, source], [
                result[source, target],
                result[target, source],
            ]
        return result
