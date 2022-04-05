from abc import abstractmethod
from typing import Protocol

from networkx import Graph


class Device(Protocol):
    @property
    @abstractmethod
    def device(self) -> Graph:
        ...
