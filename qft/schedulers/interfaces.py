from abc import abstractmethod
from typing import Any, Dict, List, Protocol, Sequence

from qft.common import QubitOp
from qft.dependencies import Dependency


class Scheduler(Protocol):
    dependency: Dependency

    @abstractmethod
    def order(self) -> Sequence[QubitOp]:
        ...

    @abstractmethod
    def instructions(self) -> List[Dict[str, Any]]:
        ...
