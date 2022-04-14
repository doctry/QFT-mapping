from qft.common import CompiledOp, CompiledProgram
from qft.deps import Dependency
from qft.devs import Device

from .distances import APSPScheduler
from .interfaces import Timing


class GreedyScheduler(APSPScheduler):
    def __init__(self, dep: Dependency, dev: Device, timing: Timing) -> None:
        super().__init__(dep, dev, timing)

    def schedule(self) -> CompiledProgram:
        return super().schedule()
