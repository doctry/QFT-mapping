from __future__ import annotations

import typing
from codeop import Compile
from dataclasses import dataclass
from typing import List, Tuple

from typing_extensions import Self

from qft import common

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

        if source >= target:
            raise ValueError(
                f"Got source: {source} >= target: {target}."
                " "
                "Source should always be smaller than target."
            )

    def json(self) -> Json:
        result = {"source": self.source, "target": self.target}
        common.union_if(result, tag=self.tag)
        return result


@dataclass
class CompiledOp(JsonSerDe):
    operator: str
    logical: Tuple[int, int] | None = None
    physical: Tuple[int, int] | None = None
    duration: Tuple[int, int] | None = None

    def json(self) -> Json:
        result = {"Operator": self.operator}

        common.union_if(
            result,
            logical=self.logical,
            physical=self.physical,
            duration=self.duration,
        )

        return result


@dataclass
class CompiledProgram(JsonSerDe):
    operations: List[CompiledOp]
    cost: int

    def json(self) -> Json:
        self.sort()
        return {
            "Operations": [op.json() for op in self.operations],
            "final_cost": self.cost,
        }

    @staticmethod
    def _check_duration(op: CompiledOp) -> Tuple[int, int]:
        assert op.duration is not None
        return op.duration

    def sort(self) -> None:

        self.operations.sort(key=self._check_duration)

    @classmethod
    def from_ops(cls, ops: List[CompiledOp]) -> Self:
        cost = max(map(lambda op: cls._check_duration(op)[1], ops))
        program = CompiledProgram(ops, cost)
        program.sort()
        return program
