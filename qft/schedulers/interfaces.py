from abc import abstractmethod
from typing import Protocol, Sequence

from qft.common import QubitOp
from qft.dependencies import Dependency
from qft.devices.interfaces import Device


class Scheduler(Protocol):
    dep: Dependency
    dev: Device

    @abstractmethod
    def schedule(self) -> Sequence[QubitOp]:
        ...
