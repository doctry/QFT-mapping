from __future__ import annotations

import json
from abc import abstractmethod
from pathlib import Path
from typing import Any, List, Mapping, Protocol, Union

Json = Union[None, bool, int, float, str, List[Any], Mapping[str, Any]]


def read(filename: str | Path) -> Json:
    with open(filename, "r") as f:
        return json.load(f)


def write(obj: Json, filename: str | Path) -> None:
    with open(filename, "w+") as f:
        json.dump(obj, f)


class JsonSerDe(Protocol):
    @abstractmethod
    def json(self) -> Json:
        ...
