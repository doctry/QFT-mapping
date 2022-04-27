from typing import List, Tuple

from qft.common.ops import CompiledOp
from qft.duostra import duostra
from qft.routers import DuostraRouter
from qft.schedulers import Scheduler

from .interfaces import Controller


class DuostraController(Controller):
    def __init__(
        self,
        device: duostra.DeviceCpp,
        router: DuostraRouter,
        scheduler: Scheduler,
    ):
        self.device = device
        self.router = router
        self.scheduler = scheduler

    def compile_route(self, route: Tuple[List[int], List[int]]) -> List[CompiledOp]:
        opers = self.device.compile_route(route)
        ret = [
            CompiledOp(
                op.get_operator_name(),
                logical=None,
                physical=op.get_qubits(),
                duration=op.get_duration,
            )
            for op in opers
        ]
        return ret
