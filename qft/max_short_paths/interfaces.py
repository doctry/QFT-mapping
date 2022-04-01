from __future__ import annotations

from abc import abstractmethod
from typing import Any, Dict, List, Protocol


class JsonSerDe(Protocol):
    @abstractmethod
    def to_json(self) -> Dict[str, Any] | List[Any] | int | float | bool | str | None:
        ...

    def __str__(self) -> str:
        return str(self.to_json())


class SerDeGraph(JsonSerDe):
    def to_json(self):
        directed = getattr(self, "is_directed")
        return {
            "directed": directed() if callable(directed) else directed,
            "nodes": getattr(self, "nodes"),
            "edges": getattr(self, "edges"),
        }
