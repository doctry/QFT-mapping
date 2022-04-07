from abc import ABCMeta, abstractmethod
from typing import Dict, Sequence, Tuple

import networkx as nx
import numpy as np
from numpy import ndarray

from qft.common import QubitOp
from qft.deps import Dependency
from qft.devs import Device

from .interfaces import Scheduler


class APSPScheduler(Scheduler, ABCMeta):
    def __init__(self, dep: Dependency, dev: Device) -> None:
        self.dep = dep
        self.dev = dev

        (self.shortest_paths, self.distances) = self._shortest_paths()

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

    @abstractmethod
    def schedule(self) -> Sequence[QubitOp]:
        ...
