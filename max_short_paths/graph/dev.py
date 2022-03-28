from __future__ import annotations

import json
from pathlib import Path
from typing import Any, Dict, List, Sequence, Tuple

import networkx as nx
from networkx import Graph
from scipy.sparse import csgraph
from typing_extensions import Self

from max_short_paths.interfaces import SerDeGraph


class DeviceGraph(Graph, SerDeGraph):
    def __init__(self, json: List[Dict[str, Any]]):
        super().__init__()

        self.add_nodes_from(range(len(json)))
        for (idx, info) in enumerate(json):
            assert idx == info["id"], [idx, info["id"]]

            for adj in info["adj_list"]:
                self.add_edge(idx, adj)

        nx.freeze(self)

    @classmethod
    def from_file(cls, file: str | Path) -> Self:
        with open(file) as f:
            data = json.load(f)
        return cls(data)


class DeviceDriver:
    def __init__(self, device: DeviceGraph) -> None:
        self.device = device
        self.mapping = {i: i for i in range(len(device.nodes))}
        self.paths = nx.shortest_path(self.device)
        self.distance = csgraph.shortest_path(nx.to_scipy_sparse_array(self.device))
        self.midpoints = self._init_midpoints()

    def __str__(self) -> str:
        return str(self.to_json())

    def path(self, source: int, target: int) -> Sequence[int]:
        return self.paths[source][self.mapping[target]]

    def to_json(self):
        return {
            "device": self.device.to_json(),
            "mapping": self.mapping,
            "paths": self.paths,
            "distance": self.distance.tolist(),
            "midpoints": self.midpoints,
        }

    def rotate(self, rotation: List[int]) -> List[int]:
        if not rotation:
            return []

        first = rotation[0]
        shifted = rotation[1:]
        shifted.append(first)
        return shifted

    def _get_midpoint(self, path: List[int]) -> Tuple[int, int]:
        if not path:
            raise ValueError(f"The list provided: {path} is empty.")

        source = path[0]
        target = path[-1]

        if source == target:
            return (source, target)

        half = self.distance[source, target] / 2

        tot_dist = 0
        for step in zip(path[:-1], path[1:]):
            tot_dist += self.distance[step]
            if tot_dist >= half:
                assert self.distance[step] > 0
                assert self.distance[source, step[0]] <= half, [
                    self.distance[source, step[0]],
                    self.distance[step],
                    self.distance[source, target],
                ]
                assert self.distance[step[1], target] <= half, [
                    self.distance[step[1], target],
                    self.distance[step],
                    self.distance[source, target],
                ]
                return step

        raise ValueError(
            f"Unreachable point. Distance matrix should be all positive: {self.distance}"
        )

    def _init_midpoints(self) -> Dict[Tuple[int, int], Tuple[int, int]]:
        results = {}
        for (source, target_path) in self.paths.items():
            for (target, path) in target_path.items():
                assert path[0] == source, [source, path]
                assert path[-1] == target, [source, path]

                midpoint = self._get_midpoint(path)
                results[source, target] = midpoint

        return results
