from networkx import DiGraph

from qft.common.ops import QubitOp

from .interfaces import Consumer, Dependency


class QFTDependency(DiGraph, Dependency):
    def __init__(self, size: int) -> None:
        super().__init__()

        for i in range(size - 1):
            for j in range(i + 1, size):
                self.__add_dependency(i, j)

    def __add_dependency(self, i: int, j: int) -> None:
        assert i < j, [i, j]

        current = QubitOp(i, j)
        self.add_node(current)

        if i >= 1:
            self.add_edge(QubitOp(i - 1, j), current)

        self.add_edge(QubitOp(i, j - 1), current)

    @property
    def dependency(self) -> DiGraph:
        return self

    def consumer(self) -> Consumer:
        return super().consumer()
