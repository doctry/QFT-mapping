import loguru
import networkx as nx
from networkx import DiGraph

from qft.common import QubitOp

from .interfaces import Consumer, Dependency


class QFTDependency(DiGraph, Dependency):
    def __init__(self, size: int) -> None:
        super().__init__()

        for i in range(size - 1):
            for j in range(i + 1, size):
                self.__add_dependency(i, j)

        loguru.logger.debug(self.json())

        nx.freeze(self)

    def __add_dependency(self, i: int, j: int) -> None:
        loguru.logger.trace("__add_dependency(i={}, j={})", i, j)

        assert i < j, [i, j]

        current = QubitOp(i, j)
        self.add_node(current)

        if i >= 1:
            self.add_edge(QubitOp(i - 1, j), current)

        if j > i + 1:
            self.add_edge(QubitOp(i, j - 1), current)

    @property
    def g(self) -> DiGraph:
        return self

    def consumer(self) -> Consumer:
        done = set()
        ready = set()
        blocked = set()

        first_op = QubitOp(0, 1)
        ready.add(first_op)

        for node in self.nodes:
            if node not in ready:
                blocked.add(node)

        loguru.logger.debug("Done={}, Ready={}, Blocked={}", done, ready, blocked)
        return Consumer(self, done, ready, blocked)
