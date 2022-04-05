from typing import Protocol, Sequence

from qft.devices import Device


class DeviceDriver(Protocol):
    device: Device
    mapping: Sequence[int]
