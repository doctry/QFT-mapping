from __future__ import annotations

from abc import abstractmethod
from dataclasses import dataclass
from typing import Any, Dict, Mapping, Protocol, Sequence

from .json import Json, JsonSerDe


@dataclass(eq=True, frozen=True, order=True)
class QubitOp(JsonSerDe):
    source: int
    target: int

    tag: str | None = None

    def __init__(self, source: int, target: int, tag: str | None = None) -> None:
        object.__setattr__(self, "source", source)
        object.__setattr__(self, "target", target)
        object.__setattr__(self, "tag", tag)

    def compile(self, compiler: Compiler) -> Sequence[Mapping[str, Any]]:
        return compiler.compile(self)

    def json(self) -> Json:
        result = {"source": self.source, "target": self.target}

        if self.tag is not None:
            result |= {"tag": self.tag}

        return result


class Compiler(Protocol):
    @abstractmethod
    def compile(self, op: QubitOp) -> Sequence[Mapping[str, Any]]:
        ...
