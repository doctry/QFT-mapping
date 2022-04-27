# pyright: reportGeneralTypeIssues=false
from typing import List, Tuple

from qft.duostra import duostra

from .interfaces import Router


class DuostraRouter(Router):
    def __init__(self, device: duostra.DeviceCpp) -> None:
        self.device = device

    def route_physical(self, source: int, target: int) -> Tuple[List[int], List[int]]:
        return self.device.route(source, target)
