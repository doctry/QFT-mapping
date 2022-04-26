from abc import abstractmethod
from typing import NamedTuple, Protocol

from qft.common import CompiledProgram
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

        result = []
        for _ in range(total_dependencies):
            operation = self.scheduler.next_op()
            route = self.router.route(operation.source, operation.target)
            result.append(route)

        assert self.scheduler.done

        raise NotImplementedError
