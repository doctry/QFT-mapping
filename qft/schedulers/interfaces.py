from abc import abstractmethod
from typing import Any, Dict, List, Protocol, Sequence, Tuple

from qft.common import Compiler, QubitOp
from qft.dependencies import Dependency
from qft.devices.interfaces import Device


class Scheduler(Compiler, Protocol):
    dependency: Dependency
    device: Device

    @abstractmethod
    def schedule(self) -> Sequence[QubitOp]:
        ...

    def compile(self) -> List[Dict[str, Any]]:
        schedule = self.schedule()
        return sum((op.compile(self) for op in schedule), [])
