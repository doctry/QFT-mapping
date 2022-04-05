from typing import List, Protocol

from qft.devices import Device


class DeviceDriver(Protocol):
    device: Device
    mapping: List[int]
