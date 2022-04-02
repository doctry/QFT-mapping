from abc import abstractmethod
from typing import Iterable, Protocol


class Device(Protocol):
    @property
    @abstractmethod
    def nodes(self) -> Iterable:
        pass
