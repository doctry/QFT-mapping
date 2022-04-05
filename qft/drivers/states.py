from typing import List

from qft.devices import Device

from .interfaces import DeviceDriver


class VanillaState(DeviceDriver):
    def __init__(self, device: Device, mapping: List[int]) -> None:
        self.device = device
        self.mapping = mapping
