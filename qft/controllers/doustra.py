from typing import List, Tuple
from .interfaces import Controller, Timing
from qft.common import CompiledProgram
from qft.common.ops import CompiledOp
from qft.routers import Router
from qft.schedulers import Scheduler
from qft.duostra import duostra

class DuostraController(Controller):
    def __init__(self, device: duostra.DeviceCpp, router: Router, scheduler: Scheduler, timing: Timing):
        self.device = device
        self.router = router
        self.scheduler = scheduler
        self.timing = timing

    def compile_route(self, route: Tuple[List[int], List[int]]) -> List[CompiledOp]:
        return self.device.compile_route(route)