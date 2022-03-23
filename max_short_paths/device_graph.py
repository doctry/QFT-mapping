from __future__ import annotations
from typing import Any, Dict, List
from networkx import Graph


class DeviceGraph(Graph):
    def __init__(self, json: List[Dict[str, Any]]):
        super().__init__()

        self.add_nodes_from(range(len(json)))
        for (idx, info) in enumerate(json):
            assert idx == info["id"], [idx, info["id"]]

            for adj in info["adj_list"]:
                self.add_edge(idx, adj)
