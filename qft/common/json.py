from __future__ import annotations

from abc import abstractmethod
from ctypes import Union
from typing import Any, List, Mapping, Protocol, Union

Json = Union[None, bool, int, float, str, List[Any], Mapping[str, Any]]


class JsonSerDe(Protocol):
    @abstractmethod
    def json(self) -> Json:
        ...
