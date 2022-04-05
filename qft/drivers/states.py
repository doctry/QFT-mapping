import copy
from typing import Any, Dict, List, Sequence

from qft.common import Compiler, QubitOp
from qft.devices import Device

from .interfaces import DeviceDriver


class VanillaState(DeviceDriver):
    def __init__(self, device: Device, mapping: List[int]) -> None:
        self.device = device
        self.mapping = mapping

    def rotate(self, indices: List[int], *, right: bool) -> None:
        rotated = self._rotate(indices, right=right)
        cloned_map = copy.copy(self.mapping)

        for (idx, ridx) in zip(indices, rotated):
            self.mapping[idx] = cloned_map[ridx]

    @staticmethod
    def _rotate(indices: List[int], *, right: bool) -> List[int]:
        if right:
            return indices[-1:] + indices[:-1]
        else:
            return indices[1:] + indices[:1]


class DistanceState(DeviceDriver):
    def __init__(self, device: Device, mapping: List[int]) -> None:
        self.device = device
        self.mapping = mapping
