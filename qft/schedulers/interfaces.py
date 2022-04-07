from abc import abstractmethod
from typing import Protocol, Sequence

from qft.common import CompiledProgram, Json, QubitOp
from qft.deps import Dependency
from qft.devs import Device


class Scheduler(Protocol):
    dep: Dependency
    dev: Device

    @abstractmethod
    def schedule(self) -> CompiledProgram:
        ...
