from abc import abstractmethod
from typing import Protocol

from qft.common.ops import QubitOp
from qft.deps import Dependency


class Scheduler(Protocol):
    dep: Dependency

    def __next__(self) -> QubitOp:
        return self.next_op()

    @abstractmethod
    def next_op(self) -> QubitOp:
        ...

    @property
    @abstractmethod
    def done(self) -> bool:
        pass
