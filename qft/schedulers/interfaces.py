from abc import abstractmethod
from typing import Protocol, Sequence

from qft.common import QubitOp
from qft.common.json import Json
from qft.dependencies import Dependency
from qft.devices.interfaces import Device


class Scheduler(Protocol):
    dep: Dependency
    dev: Device

    @abstractmethod
    def schedule(self) -> Sequence[QubitOp]:
        ...


class Compiler(Scheduler, Protocol):
    @abstractmethod
    def compile(self) -> Json:
        ...
