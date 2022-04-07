from __future__ import annotations

from dataclasses import dataclass
from typing import List, Tuple

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

        if self.tag is not None:
            result |= {"tag": self.tag}

        return result


@dataclass
class CompiledOp(JsonSerDe):
    operator: str
    qubits: Tuple[int, int]
    duration: Tuple[int, int]

    def json(self) -> Json:
        return {
            "Operator": self.operator,
            "Qubits": list(self.qubits),
            "duration": list(self.duration),
        }


@dataclass
class CompiledProgram(JsonSerDe):
    operations: List[CompiledOp]
    cost: int

    def json(self) -> Json:
        return {
            "Operations": [op.json() for op in self.operations],
            "final_cost": self.cost,
        }
