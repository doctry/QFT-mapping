from abc import abstractmethod
from typing import Any, Iterable, Mapping, Protocol, Tuple

from qft.common import QubitOp


class Device(Protocol):
    @property
    @abstractmethod
    def nodes(self) -> Iterable[QubitOp]:
        ...

    @property
    @abstractmethod
    def edges(self) -> Iterable[Tuple[QubitOp, QubitOp]]:
        ...

    @property
    @abstractmethod
    def adj(self) -> Mapping[QubitOp, Mapping[QubitOp, Mapping[str, Any]]]:
        ...
