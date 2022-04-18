from __future__ import annotations

import json
from abc import abstractmethod
from pathlib import Path
from typing import Any, List, Mapping, Protocol, Union

import loguru

Json = Union[None, bool, int, float, str, List[Any], Mapping[str, Any]]


def read(filename: str | Path) -> Json:
    with open(filename, "r") as f:
        data = json.load(f)
    loguru.logger.debug(data)
    return data


def write(obj: Json, filename: str | Path) -> None:
    loguru.logger.debug(obj)
    with open(filename, "w+") as f:
        json.dump(obj, f)


class JsonSerDe(Protocol):
    @abstractmethod
    def json(self) -> Json:
        ...
