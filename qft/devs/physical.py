from __future__ import annotations

from pathlib import Path
from typing import Any, Dict, List, Mapping

import networkx as nx
from networkx import Graph
from typing_extensions import Self

from qft import common
from qft.common import Json, JsonSerDe, TwoWayDict

from .interfaces import Device


class PhysicalDevice(Graph, Device, JsonSerDe):
    def __init__(self, data: List[Dict[str, Any]]) -> None:
        super().__init__()

        mp: List[int] = [d["id"] for d in data]
        mp_dict = dict(enumerate(mp))

        self._mapping = TwoWayDict(mp_dict)
        self.add_nodes_from(self._mapping)

        for d in data:
            id = d["id"]

            adj_list = d["adj_list"]
            assert isinstance(adj_list, List), adj_list

            for adj in adj_list:
                self.add_edge(id, adj)

        nx.freeze(self)

    def mapping(self) -> Dict[int, int]:
        return self._mapping.forward

    @property
    def reverse(self) -> Dict[int, int]:
        return self._mapping.reverse

    @property
    def g(self) -> Graph:
        return self

    def json(self) -> Json:
        return {"nodes": self.nodes, "edges": self.edges}

    @classmethod
    def from_file(cls, filename: Path | str) -> Self:
        data = common.read_json(filename)

        assert isinstance(data, List), data
        for d in data:
            assert isinstance(d, Dict), d

        return cls(data)
