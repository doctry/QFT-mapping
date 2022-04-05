from typing import Any, Dict, List, Sequence

from qft.common import QubitOp
from qft.dependencies import Dependency
from qft.devices import Device

from .interfaces import Scheduler


class BaseLineScheduler(Scheduler):
    def __init__(self, dep: Dependency, dev: Device) -> None:
        self.dep = dep
        self.dev = dev

    def schedule(self) -> Sequence[QubitOp]:
        return super().schedule()
