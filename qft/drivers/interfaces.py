from typing import List, Protocol, Sequence

from qft.common import Compiler
from qft.devices import Device


class DeviceDriver(Protocol):
    device: Device
    mapping: List[int]
