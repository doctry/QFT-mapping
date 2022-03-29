from __future__ import annotations

import itertools
import json
from pathlib import Path
from typing import Any, Dict, Iterable, List, Sequence, Tuple

import loguru
import networkx as nx
import numpy as np
from networkx import Graph
from numpy import ndarray
from scipy.sparse import csgraph
from typing_extensions import Self

from max_short_paths.interfaces import SerDeGraph

from .dep import QuBitOp


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
        self.paths = self._init_paths()
        self.distance = csgraph.shortest_path(nx.to_scipy_sparse_array(self.device))

        self.midpoints = self._init_midpoints()

        self.midpaths = self._init_midpaths()
        self.middist = self._get_midpoint_dist()

        assert not self.device.is_directed()

        loguru.logger.debug("Distance: \n {}", np.matrix(self.distance))
        loguru.logger.debug("Device: \n {}", np.matrix(nx.to_numpy_array(self.device)))

        loguru.logger.debug("Paths: {}", self.paths)
        loguru.logger.debug("Mid points: {}", self.midpoints)
        loguru.logger.debug("Mid distance: {}", self.middist)

    def __str__(self) -> str:
        return str(self.to_json())

    def perceived_distance(self, op: QuBitOp) -> Sequence[int]:
        source = self.mapping[op.source]
        target = self.mapping[op.target]
        return self.paths[source, target]

    def to_json(self):
        return {
            "device": self.device.to_json(),
            "mapping": self.mapping,
            "paths": self.paths,
            "distance": self.distance.tolist(),
            "midpoints": self.midpoints.tolist(),
            "middist": self.middist,
            "midpaths": self.midpaths,
        }

    def _rotate(self, rotation: List[int], first: bool = True) -> List[int]:
        "Rotates the qubits but also update mapping."

        if not rotation:
            return []

        if first:
            head = rotation[0]
            shifted = rotation[1:]
            shifted.append(head)
            return shifted
        else:
            head = rotation[-1]
            return [head] + rotation[:-1]

    def rotate(self, rotation: List[int], first: bool) -> None:
        "Rotate and update mapping."

        if not rotation:
            return

        rotated = self._rotate(rotation, first=first)
        assert len(rotation) == len(rotated), [rotation, rotated]

        for (f, t) in zip(rotation, rotated):
            self.mapping[f] = t

    def swap(self, op: QuBitOp) -> None:
        (source, target) = op

        source2mid = self.midpaths[source, target]
        target2mid = self.midpoints[target, source]

        self.rotate(source2mid, first=False)
        self.rotate(target2mid, first=False)

    def _init_paths(self) -> Dict[Tuple[int, int], List[int]]:
        shortest_paths = nx.shortest_path(self.device)
        loguru.logger.debug("Shortest paths: {}", shortest_paths)

        dd = {}

        for (source, target_path) in shortest_paths.items():
            for (target, path) in target_path.items():
                dd[source, target] = path

        return dd

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

    def _init_midpoints(self) -> ndarray:
        dim = len(self.device.nodes)
        results = np.full([dim, dim, 2], -1)

        for ((source, target), path) in self.paths.items():
            assert path[0] == source, [source, path]
            assert path[-1] == target, [source, path]

            midpoint = self._get_midpoint(path)
            results[source, target] = midpoint

        return results

    def _get_midpoint_dist(self) -> ndarray:
        dim = len(self.device.nodes)
        dd = np.full([dim, dim], np.inf)

        (a, b, c) = self.midpoints.shape
        assert a == b == dim, [a, b, dim]
        assert c == 2, c

        for (source, target) in itertools.product(range(dim), range(dim)):
            (mid_src, mid_tgt) = self.midpoints[source, target]
            larger = max(self.distance[source, mid_src], self.distance[mid_tgt, target])
            dd[source, target] = dd[target, source] = larger

        return dd

    def _init_midpaths(self) -> Dict[Tuple[int, int], List[int]]:
        def _collect_until(it: Iterable[int], target: int) -> List[int]:
            result = []

            for i in it:
                result.append(i)

                if i == target:
                    return result

            raise ValueError("Iterable does not contain target")

        results = {}

        for ((source, target), paths) in self.paths.items():

            (mids, midt) = self.midpoints[source, target]
            assert mids in paths, [mids, paths]
            assert midt in paths, [midt, paths]
            results[source, target] = _collect_until(paths, mids)
            results[target, source] = _collect_until(reversed(paths), midt)

        return results
