from typing import Protocol

from qft.devices import Device


class Driver(Protocol):
    device: Device
