from abc import abstractmethod
from typing import List, Protocol, Tuple

from qft.devs import Device


class Router(Protocol):
    device: Device

    def route(self, source: int, target: int, physical: bool = False) -> List[int]:
        projector = self.device.mapping

        if not physical:
            source = projector.mapping[source]
            target = projector.mapping[target]

        physical_route = self.route_physical(source, target)

        if physical:
            route = physical_route
        else:
            route = [projector.reverse[node] for node in physical_route]

        return route

    @abstractmethod
    def route_physical(self, source: int, target: int) -> List[int]:
        ...
