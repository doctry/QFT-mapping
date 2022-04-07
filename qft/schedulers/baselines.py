import random
from typing import Sequence

import numpy as np

from qft.common import QubitOp
from qft.dependencies import Dependency
from qft.devices import Device

from .distances import APSPScheduler


class BaseLineScheduler(APSPScheduler):
    def __init__(self, dep: Dependency, dev: Device) -> None:
        super().__init__(dep, dev)

        self.max_dist = np.max(self.distances)

    def schedule(self) -> Sequence[QubitOp]:
        consumer = self.dep.consumer()
        history = []

        while not consumer.terminate:
            available = random.choice(tuple(consumer.ready))
            consumer.process(available)
            history.append(available)

        return history
