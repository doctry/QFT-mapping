from abc import abstractmethod
from typing import List, Protocol, Tuple

from qft.devs import Device


class Router(Protocol):
    device: Device

    @abstractmethod
    def route(self, source: int, target: int, physical: bool = False) -> List[int]:
        ...
