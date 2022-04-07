from typing import List

import numpy as np

from qft.common import CompiledProgram, QubitOp
from qft.deps import Dependency
from qft.devs import Device

from .distances import APSPScheduler


class BaselineScheduler(APSPScheduler):
    def __init__(self, dep: Dependency, dev: Device) -> None:
        super().__init__(dep, dev)

        self.max_cost = np.max(self.distances)

    def schedule(self) -> CompiledProgram:
        history = self._history()

        program = []
        current_cost = 0

        for qop in history:
            ops = self.execute(qop, current_cost)
            program.extend(ops)

        raise NotImplementedError

    def _history(self) -> List[QubitOp]:
        consumer = self.dep.consumer()
        history = []

        while not consumer.terminate:
            available = consumer.ready.pop()
            consumer.process(available)
            history.append(available)

        assert consumer.terminate, consumer
        assert len(history) == len(self.dep.dependency.nodes), history
        return history


class SynchronousScheduler(BaselineScheduler):
    def __init__(self, dep: Dependency, dev: Device) -> None:
        super().__init__(dep, dev)
