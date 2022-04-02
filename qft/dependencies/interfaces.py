import graphlib
from abc import abstractmethod
from typing import Protocol, Set

import loguru
from networkx import DiGraph

from qft.common import QubitOp


class Dependency(Protocol):
    graph: DiGraph


class Consumer(Protocol):
    dependency: Dependency

    done: Set[QubitOp]
    ready: Set[QubitOp]
    blocked: Set[QubitOp]

    terminate: bool

    def __len__(self) -> str:
        return len(self.ready) + len(self.blocked)

    def __str__(self) -> str:
        ...

    @staticmethod
    def __transfer(op: QubitOp, source: Set[QubitOp], target: Set[QubitOp]) -> None:
        assert op in source, [op, source]
        source.remove(op)
        target.add(op)

    def process(self, op: QubitOp) -> None:
        if op not in self.ready:
            raise ValueError(
                f"Op: {op} not ready yet. Pick from the ready queue: {self.ready}."
            )

        loguru.logger.debug("Processing: {} from {}.", op, self)

        self.__transfer(op, self.ready, self.done)

        dep_graph = self.dependency.graph

        blocked_ops = dep_graph.adj[op]

        # If node is blocked ==> move to ready if all in-nodes are done.
        for op in blocked_ops:
            if op in self.blocked and (
                all(in_node in self.done) for (in_node, _) in dep_graph.in_edges(op)
            ):
                self.__transfer(op, self.ready, self.done)

        loguru.logger.debug(self)
        assert len(self) + len(self.done) == len(self.graph.nodes)
