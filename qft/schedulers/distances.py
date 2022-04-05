from typing import Mapping, Sequence

import networkx as nx

from qft.dependencies import Dependency
from qft.devices import Device

from .interfaces import Scheduler


class APSPScheduler(Scheduler):
    def __init__(self, dep: Dependency, dev: Device) -> None:
        self.dep = dep
        self.dev = dev

        self.shortest_paths: Mapping[
            int, Mapping[int, Sequence[int]]
        ] = nx.shortest_path(self.dev.device)
