from __future__ import annotations

from abc import abstractmethod
from typing import Any, Dict, List, Protocol

from networkx import Graph


class JsonSerDe(Protocol):
    @abstractmethod
    def to_json(self) -> Dict[str, Any] | List[Any] | int | float | bool | str | None:
        ...

    def __str__(self) -> str:
        return str(self.to_json())


class SerDeGraph(Graph, JsonSerDe):
    def to_json(self):
        return {"directed": self.is_directed, "nodes": self.nodes, "edges": self.edges}
