from __future__ import annotations

from abc import abstractmethod
from typing import List, NamedTuple, Protocol, Tuple

from qft.common import CompiledOp, CompiledProgram
from qft.devs import Device
from qft.routers import Router
from qft.schedulers import Scheduler


class Timing(NamedTuple):
    swap: int
    operation: int


class Controller(Protocol):
    device: Device
    scheduler: Scheduler
    router: Router
    timing: Timing | None

    def compile(self) -> CompiledProgram:
        nodes = max(self.scheduler.dep.g.nodes, key=lambda x: x.target).target + 1
        total_dependencies = (nodes * (nodes - 1)) // 2

        ops: List[CompiledOp] = []
        for _ in range(total_dependencies):
            operation = next(self.scheduler)
            route = self.router.route(
                operation.source, operation.target, already_physical=False
            )
            compiled_route = self.exec_route(route)
            ops.extend(compiled_route)

        assert self.scheduler.done, self.scheduler

        return CompiledProgram.from_ops(ops)

    def exec_route(
        self, route: Tuple[List[int], List[int]], already_physical: bool
    ) -> List[CompiledOp]:
        compiled = self.compile_route(route, already_physical=already_physical)
        self.device.rotate(route[0], right=False, already_physical=already_physical)
        self.device.rotate(route[1], right=False, already_physical=already_physical)
        return compiled

    @abstractmethod
    def compile_route(
        self, route: Tuple[List[int], List[int]], already_physical: bool
    ) -> List[CompiledOp]:
        ...
