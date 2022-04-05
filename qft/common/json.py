from __future__ import annotations

from abc import abstractmethod
from ctypes import Union
from typing import Dict, List, Protocol, Union

Json = Union[None, bool, int, float, str, List["Json"], Dict[str, "Json"]]


class JsonSerDe(Protocol):
    @abstractmethod
    def json(self) -> Json:
        ...
