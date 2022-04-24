from abc import abstractmethod
from typing import NamedTuple, Protocol

from qft.common import CompiledProgram
from qft.common.ops import QubitOp
from qft.deps import Dependency
from qft.devs import Device


class Scheduler(Protocol):
    dep: Dependency

    @abstractmethod
    def next_op(self) -> QubitOp:
        ...

    @property
    @abstractmethod
    def done(self) -> bool:
        pass
