from __future__ import annotations

import typing
from typing import Any, Dict, FrozenSet, List, NamedTuple, Set, Tuple

import loguru
import networkx as nx
from networkx import DiGraph

from .interfaces import SerDeGraph


class OperationEdge(NamedTuple):
    source: int
    target: int

    def __eq__(self, other: Any) -> bool:
        if not isinstance(other, (OperationEdge, Tuple)):
            return False

        if len(other) != 2:
            return False

        other = OperationEdge(*other)

        return (
            self.source == other.source
            and self.target == other.target
            or self.source == other.target
            and self.target == other.source
        )

    def to_json(self) -> List[int]:
        return [self.source, self.target]


class DependencyGraph(DiGraph, SerDeGraph):
    def __init__(self, n: int) -> None:
        super().__init__()

        for i in range(n - 1):
            for j in range(i + 1, n):
                edge = OperationEdge(i, j)
                self.add_node(edge)

                if (prev := OperationEdge(i - 1, j)) in self.nodes:
                    self.add_edge(prev, edge)

                if (prev := OperationEdge(i, j - 1)) in self.nodes:
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


class Consumer:
    """
    Each node in the consumer must be in one of the four sets:

    - done
        Nodes that are already processed.

    - ready
        Nodes that can be processed in the next turn.

    - waiting
        Nodes that can be ready if a node is moved from ready to done.
        This set is checked if it could be ready, so ideally as small as possible.

    - blocked
        Nodes that cannot be processed.
    """

    def __init__(self, graph: DependencyGraph) -> None:
        self._graph = graph

        # Initially, all nodes are not processed.
        # Except the first node is ready.
        self._done: Set[OperationEdge] = set()
        self._ready: Set[OperationEdge] = set()
        self._waiting: Set[OperationEdge] = set()
        self._blocked: Set[OperationEdge] = set()

        self._init_sets()

        loguru.logger.debug(self.to_json())

    def __str__(self) -> str:
        return str(self.to_json())

    def to_json(self) -> Dict[str, List[List[int]]]:
        return {
            "done": sorted([e.to_json() for e in self.done]),
            "ready": sorted([e.to_json() for e in self.ready]),
            "waiting": sorted([e.to_json() for e in self.waiting]),
            "blocked": sorted([e.to_json() for e in self.blocked]),
        }

    def _init_sets(self) -> None:
        """
        Initialize the sets.
        """

        if len(self.graph.nodes) == 0:
            return

        first_node = OperationEdge(0, 1)
        assert first_node in self.graph.nodes, self.graph.nodes
        self._ready.add(first_node)
        loguru.logger.debug(f"Ready: {self.ready}")

        for node in self.graph.adj[first_node]:
            self._waiting.add(node)
        loguru.logger.debug(f"Waiting: {self.waiting}")
        if len(self.graph.nodes) == 1:
            assert len(self._waiting) == 0, self._waiting
        else:
            assert len(self._waiting) == 1, self._waiting

        for node in self.graph.nodes:
            if node in self.ready or node in self.waiting:
                continue
            self._blocked.add(node)
        loguru.logger.debug(f"Blocked: {self.blocked}")

    def __len__(self) -> int:
        return len(self.done) + len(self.ready) + len(self.waiting) + len(self.blocked)

    @staticmethod
    def _freeze(s: Set[OperationEdge]) -> FrozenSet[OperationEdge]:
        # Using a cast because it's much cheaper than actually creating one.
        # Also, it's because I always use a type checker so modification would be recorded.
        return typing.cast(FrozenSet, s)

    @property
    def graph(self) -> DependencyGraph:
        return self._graph

    @property
    def done(self) -> FrozenSet[OperationEdge]:
        return self._freeze(self._done)

    @property
    def ready(self) -> FrozenSet[OperationEdge]:
        return self._freeze(self._ready)

    @property
    def waiting(self) -> FrozenSet[OperationEdge]:
        return self._freeze(self._waiting)

    @property
    def blocked(self) -> FrozenSet[OperationEdge]:
        return self._freeze(self._blocked)

    @property
    def finished(self) -> int:
        return len(self.done)

    @property
    def unfinished(self) -> int:
        return len(self.ready) + len(self.waiting) + len(self.blocked)

    @staticmethod
    def transfer(
        node: OperationEdge, source: Set[OperationEdge], target: Set[OperationEdge]
    ) -> None:
        loguru.logger.trace(f"Moving {node} from: {source} to: {target}")
        source.remove(node)
        target.add(node)

    def visit(self, node: OperationEdge | Tuple[int, int]) -> None:
        node = OperationEdge(*node)

        if node not in self.ready:
            raise ValueError(
                f"Node: {node} not ready yet. Pick from the ready queue: {self.ready}"
            )
        loguru.logger.debug(f"Visiting: {node} from: {self}")

        self.transfer(node, self._ready, self._done)

        adj_nodes = self.graph.adj[node]
        for node in adj_nodes:
            # If node is blocked ==> move to waiting.
            if node in self.blocked:
                self.transfer(node, self._blocked, self._waiting)

            # If node is waiting ==> move to ready if all in nodes are done.
            if node in self.waiting and all(
                in_node in self.done for (in_node, _) in self.graph.in_edges(node)
            ):
                self.transfer(node, self._waiting, self._ready)

        loguru.logger.debug(self)
        assert len(self) == len(self.graph.nodes)
