from __future__ import annotations

from typing import List, Tuple

from qft.common import CompiledOp
from qft.devs import Device
from qft.routers import Router
from qft.schedulers import Scheduler

from .interfaces import Controller, Timing


class APSPController(Controller):
    def __init__(
        self, device: Device, scheduler: Scheduler, router: Router, timing: Timing
    ) -> None:
        super().__init__()

        self.device = device
        self.scheduler = scheduler
        self.router = router
        self.timing = timing

        self.available = [0] * len(self.device.g.nodes)

    def compile_route(self, route: Tuple[List[int], List[int]]) -> List[CompiledOp]:
        (r0, r1) = route
        return self._swap_path(r0) + self._swap_path(r1) + [self._op(r0[-1], r1[-1])]

    def _swap_path(self, route: List[int]) -> List[CompiledOp]:
        wall_clock = max(self.available[r] for r in route)

        ops = []
        for (s, t) in zip(route[:-1], route[1:]):
            start = wall_clock
            end = wall_clock + self.timing.swap

            self.available[s] = self.available[t] = wall_clock = end
            ops.append(
                CompiledOp(operator="Swap", physical=(s, t), duration=(start, end))
            )

    def _op(self, a: int, b: int) -> CompiledOp:
        start = max(self.available[a], self.available[b])
        end = start + self.timing.operation
        self.available[a] = self.available[b] = end
        return CompiledOp(operator="R", physical=(a, b), duration=(start, end))
