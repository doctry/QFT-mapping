from __future__ import annotations

from abc import abstractmethod
from typing import List, Protocol, Tuple

from qft.devs import Device
from qft.duostra import duostra


class Router(Protocol):
    device: Device | duostra.DeviceCpp

    def route(
        self, source: int, target: int, physical: bool = False
    ) -> Tuple[List[int], List[int]]:
        projector = self.device.mapping()

        if not physical:
            source = projector[source]
            target = projector[target]

        return self.route_physical(source, target)

    @abstractmethod
    def route_physical(self, source: int, target: int) -> Tuple[List[int], List[int]]:
        ...
