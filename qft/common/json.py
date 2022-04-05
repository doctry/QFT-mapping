from __future__ import annotations

from abc import abstractmethod
from ctypes import Union
from typing import Any, Dict, List, Protocol, Union

# While the correct type definition should be:
# Union[None, bool, int, float, str, List["Json"], Dict[str, "Json"]]
# It is not supported by the type checker.
Json = Union[None, bool, int, float, str, List[Any], Dict[str, Any]]


class JsonSerDe(Protocol):
    @abstractmethod
    def json(self) -> Json:
        ...
