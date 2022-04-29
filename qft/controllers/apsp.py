from __future__ import annotations

from typing import List, Tuple

from qft.common import CompiledOp, QubitOp
from qft.deps.interfaces import Dependency
from qft.devs import Device
from qft.routers import Router
from qft.schedulers import Scheduler

from .interfaces import Controller, Timing


class APSPMixin:
    timing: Timing
    available: List[int]

    def compile_route(self, route: Tuple[List[int], List[int]]) -> List[CompiledOp]:
        (r0, r1) = route
        return self._swap_path(r0) + self._swap_path(r1) + [self._op(r0[-1], r1[-1])]

    def _swap_path(self, route: List[int]) -> List[CompiledOp]:
        wall_clock = max(
            self.available[r] - td for (r, td) in zip(route, self._tight_path(route))
        )

        ops = []
        for (s, t) in zip(route[:-1], route[1:]):
            start = wall_clock
            end = wall_clock + self.timing.swap

            self.available[s] = self.available[t] = wall_clock = end
            ops.append(
                CompiledOp(operator="Swap", physical=(s, t), duration=(start, end))
            )
        return ops

    def _op(self, a: int, b: int) -> CompiledOp:
        start = max(self.available[a], self.available[b])
        end = start + self.timing.operation
        self.available[a] = self.available[b] = end
        return CompiledOp(operator="R", physical=(a, b), duration=(start, end))

    def _tight_path(self, route: List[int]) -> List[int]:
        time_diff = [0] + list(range(len(route) - 1))
        time_diff = [self.timing.swap * x for x in time_diff]
        return time_diff


class APSPController(APSPMixin, Controller):
    def __init__(
        self, device: Device, scheduler: Scheduler, router: Router, timing: Timing
    ) -> None:
        self.device = device
        self.scheduler = scheduler
        self.router = router
        self.timing = timing
        self.available = [0] * len(self.device.g.nodes)


class WorkStealingController(APSPMixin, Scheduler):
    def __init__(
        self, device: Device, dependency: Dependency, router: Router, timing: Timing
    ) -> None:
        self.device = device
        self.dependency = dependency
        self.consumer = self.dependency.consumer()
        self.router = router
        self.timing = timing
        self.available = [0] * len(self.device.g.nodes)

    @property
    def scheduler(self) -> Scheduler:
        return self

    def next_op(self) -> QubitOp:
        ready = tuple(self.consumer.ready)
        shortest = min(ready, key=lambda x: len(self.router.route(x.source, x.target)))
        self.consumer.process(shortest)
        return shortest

    @property
    def done(self) -> bool:
        return not len(self.consumer)
