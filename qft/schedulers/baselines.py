import random
from typing import Sequence

import numpy as np

from qft.common import QubitOp
from qft.deps import Dependency
from qft.devs import Device

from .distances import APSPScheduler


class RandomScheduler(APSPScheduler):
    def __init__(self, dep: Dependency, dev: Device) -> None:
        super().__init__(dep, dev)

        self.max_dist = np.max(self.distances)

    def schedule(self) -> Sequence[QubitOp]:
        consumer = self.dep.consumer()
        history = []

        while not consumer.terminate:
            available = consumer.ready.pop()
            consumer.process(available)
            history.append(available)

        return history


class GreedyScheduler(APSPScheduler):
    def __init__(self, dep: Dependency, dev: Device) -> None:
        super().__init__(dep, dev)
