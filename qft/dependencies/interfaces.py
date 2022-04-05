from __future__ import annotations

import json
from abc import abstractmethod
from dataclasses import dataclass
from typing import Protocol, Set

import loguru
from networkx import DiGraph

from qft.common import Json, JsonSerDe, QubitOp


class Dependency(JsonSerDe, Protocol):
    @property
    @abstractmethod
    def dependency(self) -> DiGraph:
        ...

    @abstractmethod
    def consumer(self) -> Consumer:
        ...

    def json(self) -> Json:
        dependency = self.dependency
        return {"nodes": list(dependency.nodes), "edges": list(dependency.edges)}


@dataclass(frozen=True)
class Consumer:
    dependency: Dependency

    done: Set[QubitOp]
    ready: Set[QubitOp]
    blocked: Set[QubitOp]

    def __len__(self) -> int:
        return len(self.ready) + len(self.blocked)

    def __str__(self) -> str:
        return json.dumps(
            {
                "done": self.done,
                "ready": self.ready,
                "blocked": self.blocked,
            }
        )

    @property
    def terminate(self) -> bool:
        return not len(self)

    @staticmethod
    def __transfer(op: QubitOp, source: Set[QubitOp], target: Set[QubitOp]) -> None:
        assert op in source, [op, source]
        assert op not in target, [op, target]
        source.remove(op)
        target.add(op)

    def process(self, op: QubitOp) -> None:
        if op not in self.ready:
            raise ValueError(
                f"Op: {op} not ready yet. Pick from the ready queue: {self.ready}."
            )

        loguru.logger.debug("Processing: {} from {}.", op, self)

        self.__transfer(op, self.ready, self.done)

        dep_graph = self.dependency.dependency

        blocked_ops = dep_graph.adj[op]

        # If node is blocked ==> move to ready if all in-nodes are done.
        for op in blocked_ops:
            if op in self.blocked and all(
                in_node in self.done for in_node in dep_graph.in_edges(op).keys()
            ):
                self.__transfer(op, self.ready, self.done)

        loguru.logger.debug(self)
        assert len(self) + len(self.done) == len(dep_graph.nodes)
