from typing import List, Tuple

from qft.common.ops import CompiledOp
from qft.duostra import duostra
from qft.routers import Router
from qft.schedulers import Scheduler

from .interfaces import Controller, Timing


class DuostraController(Controller):
    def __init__(
        self,
        device: duostra.DeviceCpp,
        router: Router,
        scheduler: Scheduler,
        timing: Timing,
    ):
        self.device = device
        self.router = router
        self.scheduler = scheduler
        self.timing = timing

    def compile_route(self, route: Tuple[List[int], List[int]]) -> List[CompiledOp]:
        return self.device.compile_route(route)
