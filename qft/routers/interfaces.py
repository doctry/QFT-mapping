# pyright: reportGeneralTypeIssues=false
from __future__ import annotations

from abc import abstractmethod
from typing import List, Protocol, Tuple

from qft.devs import Device
from qft.duostra import duostra


class Router(Protocol):
    device: Device | duostra.DeviceCpp

    def route(
        self, source: int, target: int, already_physical: bool = False
    ) -> Tuple[List[int], List[int]]:
        source = self.device.to_physical(source, convert_to_physical=already_physical)
        target = self.device.to_physical(target, convert_to_physical=already_physical)
        return self.route_physical(source, target)

    @abstractmethod
    def route_physical(self, source: int, target: int) -> Tuple[List[int], List[int]]:
        ...
