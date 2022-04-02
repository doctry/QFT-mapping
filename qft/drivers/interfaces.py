from abc import abstractmethod
from typing import Mapping, Protocol

from qft.devices import Device


class DeviceState(Protocol):
    device: Device
    mapping: Mapping[int, int]
