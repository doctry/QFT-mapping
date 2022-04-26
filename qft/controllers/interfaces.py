import typing
from abc import abstractmethod
from typing import List, NamedTuple, Protocol, Tuple

from qft.common import CompiledProgram
from qft.common.ops import CompiledOp
from qft.routers import Router
from qft.schedulers import Scheduler


class Timing(NamedTuple):
    swap: int
    operation: int


class Controller(Protocol):
    scheduler: Scheduler
    router: Router
    timing: Timing

    @abstractmethod
    def compile(self) -> CompiledProgram:
        nodes = len(self.scheduler.dep.g.nodes)
        total_dependencies = nodes * (nodes - 1) // 2

        ops: List[CompiledOp] = []
        for _ in range(total_dependencies):
            operation = self.scheduler.next_op()
            route = self.router.route(
                operation.source, operation.target, physical=False
            )
            compiled_route = self.compile_route(route)
            ops.extend(compiled_route)

        assert self.scheduler.done, self.scheduler

        cost = max(
            map(lambda x: tuple(typing.cast(Tuple[int, int], x.duration))[1], ops)
        )

        program = CompiledProgram(ops, cost)
        program.sort()
        return program

    @abstractmethod
    def compile_route(self, route: List[int]) -> List[CompiledOp]:
        ...
