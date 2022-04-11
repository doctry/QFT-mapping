from abc import abstractmethod
from typing import NamedTuple, Protocol

from qft.common import CompiledProgram
from qft.deps import Dependency
from qft.devs import Device


class Timing(NamedTuple):
    swap: int
    operation: int


class Scheduler(Protocol):
    dep: Dependency
    dev: Device
    timing: Timing

    @abstractmethod
    def schedule(self) -> CompiledProgram:
        ...
