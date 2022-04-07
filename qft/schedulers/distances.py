from abc import ABCMeta, abstractmethod
from typing import Dict, List, Sequence, Tuple

import networkx as nx
import numpy as np
from numpy import ndarray

from qft.common import QubitOp
from qft.common import CompiledOp
from qft.deps import Dependency
from qft.devs import Device

from .interfaces import Scheduler


class APSPScheduler(Scheduler, ABCMeta):
    def __init__(self, dep: Dependency, dev: Device) -> None:
        self.dep = dep
        self.dev = dev

        (self.shortest_paths, self.distances) = self._shortest_paths()

        self._check_symmetry()

    @abstractmethod
    def schedule(self) -> Sequence[QubitOp]:
        ...

    def execute(self, qop: QubitOp, start: int) -> List[CompiledOp]:
        path = self.shortest_paths[qop.source, qop.target]

        if len(path) <= 1:
            raise ValueError(
                f"This is impossible. The path between node {i} and {j} is {path}."
            )

        head = path[: len(qop) // 2]
        tail = path[len(qop) // 2 :]

        head_swaps = []

    @staticmethod
    def _to_swaps(sequence: Sequence[int]) -> List[CompiledOp]:
        ops = []
        for (s, t) in zip(sequence[:-1], sequence[1:]):
            ops.append(CompiledOp(operator="swap"))

    def _shortest_paths(self) -> Tuple[Dict[Tuple[int, int], Sequence[int]], ndarray]:
        sp = nx.shortest_path(self.dev.device)

        dimension = len(self.dev.device.nodes)
        distances = np.zeros(shape=[dimension, dimension], dtype=float)

        shortest_paths = {}

        for (source, target_paths) in sp.items():
            for (target, paths) in target_paths.items():
                shortest_paths[source, target] = paths
                distances[source, target] = len(paths)

        return (shortest_paths, distances)

    def _check_symmetry(self) -> None:
        for (a, b) in self.shortest_paths.keys():
            assert (b, a) in self.shortest_paths.keys(), [a, b, self.shortest_paths]
            assert self.shortest_paths[a, b] == self.shortest_paths[b, a]

            assert self.distances[a, b] == self.distances[b, a]
