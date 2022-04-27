from abc import abstractmethod
from typing import List, Protocol, Tuple

from qft.devs import Device


class Router(Protocol):
    device: Device

    def route(
        self, source: int, target: int, physical: bool = False
    ) -> Tuple[List[int], List[int]]:
        projector = self.device.mapping

        if not physical:
            source = projector.mapping[source]
            target = projector.mapping[target]

        return self.route_physical(source, target)

    @abstractmethod
    def route_physical(self, source: int, target: int) -> Tuple[List[int], List[int]]:
        ...
