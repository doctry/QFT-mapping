import random
from typing import List

from omegaconf import DictConfig

from qft.common import CompiledProgram, QubitOp
from qft.deps import Dependency
from qft.devs import Device

from .distances import APSPScheduler
from .interfaces import Timing


class BaselineScheduler(APSPScheduler):
    def __init__(
        self, cfg: DictConfig, dep: Dependency, dev: Device, timing: Timing
    ) -> None:
        super().__init__(cfg, dep, dev, timing)

        self._max_dist = self.distances.max()

    def schedule(self) -> CompiledProgram:
        history = self._history()

        program = []
        cost = 0
        time_cfg = self.cfg["time"]

        swap_time = time_cfg["swap"]
        op_time = time_cfg["op"]

        for qop in history:
            (ops, _) = self.execute(qop, cost)
            program.extend(ops)
            cost += (self._max_dist - 1) * swap_time + op_time

        return CompiledProgram(program, cost)

    def _history(self) -> List[QubitOp]:
        consumer = self.dep.consumer()
        history = []

        while not consumer.terminate:
            available = random.choice(tuple(consumer.ready))
            consumer.process(available)
            history.append(available)

        assert consumer.terminate, consumer
        assert len(history) == len(self.dep.g.nodes), history
        return history


class SynchronousScheduler(BaselineScheduler):
    def __init__(
        self, cfg: DictConfig, dep: Dependency, dev: Device, timing: Timing
    ) -> None:
        super().__init__(cfg, dep, dev, timing)

    def schedule(self) -> CompiledProgram:
        history = self._history()

        program = []
        cost = 0

        for qop in history:
            (ops, cost) = self.execute(qop, cost)
            program.extend(ops)

        return CompiledProgram(program, cost)
