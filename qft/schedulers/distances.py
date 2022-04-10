from abc import ABCMeta, abstractmethod
from typing import Dict, List, Sequence, Tuple

import networkx as nx
import numpy as np
from numpy import ndarray

from qft.common import CompiledOp, QubitOp
from qft.common.ops import CompiledProgram
from qft.deps import Dependency
from qft.devs import Device

from .interfaces import Scheduler, Timing


class APSPScheduler(Scheduler, ABCMeta):
    def __init__(self, dep: Dependency, dev: Device, timing: Timing) -> None:
        self.dep = dep
        self.dev = dev

        self.timing = timing

        (self.shortest_paths, self.distances) = self._shortest_paths()

        self._check_symmetry()

    @abstractmethod
    def schedule(self) -> CompiledProgram:
        ...

    def execute(self, qop: QubitOp, wall: int) -> Tuple[List[CompiledOp], int]:
        path = self.shortest_paths[qop.source, qop.target]

        if len(path) <= 1:
            raise ValueError(
                f"This is impossible. The path between node {qop.source} and {qop.target} is {path}."
            )

        head = path[: len(path) // 2]
        tail = path[len(path) // 2 :]

        (head_swp, head_wall) = self._to_swaps(head, wall)
        (tail_swp, tail_wall) = self._to_swaps(tail, wall)

        swp_end_wall = max(head_wall, tail_wall)
        op_end_wall = swp_end_wall + self.timing.operation

        operation = CompiledOp(
            operator="R",
            physical=(head[-1], tail[0]),
            duration=(swp_end_wall, op_end_wall),
        )

        self.dev.rotate(head, right=False)
        self.dev.rotate(tail, right=True)

        return ([*head_swp, operation, *tail_swp], op_end_wall)

    def _to_swaps(
        self, sequence: Sequence[int], wall: int
    ) -> Tuple[List[CompiledOp], int]:
        ops = []
        swap = self.timing.swap
        for (i, (s, t)) in enumerate(zip(sequence[:-1], sequence[1:])):
            duration = (wall + i * swap, wall + (i + 1) * swap)
            ops.append(CompiledOp(operator="Swap", physical=(s, t), duration=duration))
        return (ops, wall + len(sequence) * swap)

    def _shortest_paths(self) -> Tuple[Dict[Tuple[int, int], List[int]], ndarray]:
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
