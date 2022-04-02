from abc import abstractmethod
from typing import Any, Dict, List, Protocol, Sequence
from qft.dependencies import Dependency
from qft.common import QubitOp


class Scheduler(Protocol):
    dependency: Dependency

    @abstractmethod
    def order(self) -> Sequence[QubitOp]:
        ...

    @abstractmethod
    def instructions(self) -> List[Dict[str, Any]]:
        ...
