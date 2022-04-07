from abc import abstractmethod
from typing import Protocol

from qft.common import CompiledProgram
from qft.deps import Dependency
from qft.devs import Device


class Scheduler(Protocol):
    dep: Dependency
    dev: Device

    @abstractmethod
    def schedule(self) -> CompiledProgram:
        ...
