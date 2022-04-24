from abc import abstractmethod
from typing import Protocol

from qft.common.ops import QubitOp
from qft.deps import Dependency


class Scheduler(Protocol):
    dep: Dependency

    @abstractmethod
    def next_op(self) -> QubitOp:
        ...

    @property
    @abstractmethod
    def done(self) -> bool:
        pass
