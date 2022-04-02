from __future__ import annotations
from abc import abstractmethod

from typing import Any, Dict, List, NamedTuple, Protocol


class QubitOp(NamedTuple):
    name: str | None
    source: int
    target: int

    def compile(self, compiler: Compiler) -> List[Dict[str, Any]]:
        return compiler.compile(self)


class Compiler(Protocol):
    @abstractmethod
    def compile(self, op: QubitOp) -> List[Dict[str, Any]]:
        ...
