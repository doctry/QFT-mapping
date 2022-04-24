from abc import abstractmethod
from typing import NamedTuple, Protocol

from qft.common import CompiledOp, CompiledProgram
from qft.router import Router
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
        ...
