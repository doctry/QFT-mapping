from __future__ import annotations

from typing import Any, Dict, Generator, List, NamedTuple, Set, Tuple

import loguru
import networkx as nx
from networkx import DiGraph
from typing_extensions import Self

from max_short_paths.interfaces import SerDeGraph


class QuBitOp(NamedTuple):
    source: int
    target: int

    def __eq__(self, other: Any) -> bool:
        if not isinstance(other, (QuBitOp, Tuple)):
            return False

        if len(other) != 2:
            return False

        other = QuBitOp.from_tuple(other)

        return (
            self.source == other.source
            and self.target == other.target
            or self.source == other.target
            and self.target == other.source
        )

    @classmethod
    def from_tuple(cls, tup: QuBitOp | Tuple[int, int]) -> Self:
        if isinstance(tup, QuBitOp):
            return tup
        return cls(*tup)

    def to_json(self) -> List[int]:
        return [self.source, self.target]


class DependencyGraph(DiGraph, SerDeGraph):
    def __init__(self, n: int) -> None:
        super().__init__()

        for i in range(n - 1):
            for j in range(i + 1, n):
                edge = QuBitOp(i, j)
                self.add_node(edge)

                if (prev := QuBitOp(i - 1, j)) in self.nodes:
                    self.add_edge(prev, edge)

                if (prev := QuBitOp(i, j - 1)) in self.nodes:
                    self.add_edge(prev, edge)

        nx.freeze(self)

    def to_json(self) -> Any:
        return {
            "nodes": [node.to_json() for node in self.nodes],
            "edges": [[edge[0].to_json(), edge[1].to_json()] for edge in self.edges],
        }

    @property
    def consumer(self) -> Consumer:
        return Consumer(self)


class SetView:
    def __init__(self, set: Set[QuBitOp]) -> None:
        self._set = set

    def __contains__(self, elem: QuBitOp | Tuple[int, int]) -> bool:
        return QuBitOp.from_tuple(elem) in self._set

    def __len__(self) -> int:
        return len(self._set)

    def __iter__(self) -> Generator[QuBitOp, None, None]:
        for elem in self._set:
            yield elem


class Consumer:
    """
    Each node in the consumer must be in one of the three sets:

    - done
        Nodes that are already processed.

    - ready
        Nodes that can be processed in the next turn.

    - blocked
        Nodes that cannot be processed.
    """

    def __init__(self, graph: DependencyGraph) -> None:
        self._graph = graph

        # Initially, all nodes are not processed.
        # Except the first node is ready.
        self._done: Set[QuBitOp] = set()
        self._ready: Set[QuBitOp] = set()
        self._blocked: Set[QuBitOp] = set()

        self._init_sets()

        loguru.logger.debug(self.to_json())

    def __str__(self) -> str:
        return str(self.to_json())

    def to_json(self) -> Dict[str, List[List[int]]]:
        return {
            "done": sorted([e.to_json() for e in self.done]),
            "ready": sorted([e.to_json() for e in self.ready]),
            "blocked": sorted([e.to_json() for e in self.blocked]),
        }

    def _init_sets(self) -> None:
        """
        Initialize the sets.
        """

        if len(self.graph.nodes) == 0:
            return

        first_node = QuBitOp(0, 1)
        assert first_node in self.graph.nodes, self.graph.nodes
        self._ready.add(first_node)
        loguru.logger.debug(f"Ready: {self.ready}")

        for node in self.graph.nodes:
            if node in self.ready:
                continue
            self._blocked.add(node)
        loguru.logger.debug(f"Blocked: {self.blocked}")

    def __len__(self) -> int:
        return self.finished + self.unfinished

    @staticmethod
    def _freeze(s: Set[QuBitOp]) -> SetView:
        return SetView(s)

    @property
    def graph(self) -> DependencyGraph:
        return self._graph

    @property
    def done(self) -> SetView:
        return self._freeze(self._done)

    @property
    def ready(self) -> SetView:
        return self._freeze(self._ready)

    @property
    def blocked(self) -> SetView:
        return self._freeze(self._blocked)

    @property
    def finished(self) -> int:
        return len(self.done)

    @property
    def unfinished(self) -> int:
        return len(self.ready) + len(self.blocked)

    @property
    def terminate(self) -> bool:
        return self.unfinished == 0

    @staticmethod
    def transfer(
        node: QuBitOp | Tuple[int, int],
        source: Set[QuBitOp],
        target: Set[QuBitOp],
    ) -> None:
        loguru.logger.trace(f"Moving {node} from: {source} to: {target}")
        node = QuBitOp.from_tuple(node)
        source.remove(node)
        target.add(node)

    def visit(self, node: QuBitOp | Tuple[int, int]) -> None:
        node = QuBitOp.from_tuple(node)

        if node not in self.ready:
            raise ValueError(
                f"Node: {node} not ready yet. Pick from the ready queue: {self.ready}"
            )
        loguru.logger.debug(f"Visiting: {node} from: {self}")

        self.transfer(node, self._ready, self._done)

        adj_nodes = self.graph.adj[node]
        for node in adj_nodes:
            # If node is blocked ==> move to ready if all in-nodes are done.
            if node in self.blocked and all(
                in_node in self.done for (in_node, _) in self.graph.in_edges(node)
            ):
                self.transfer(node, self._ready, self._ready)

        loguru.logger.debug(self)
        assert len(self) == len(self.graph.nodes)
