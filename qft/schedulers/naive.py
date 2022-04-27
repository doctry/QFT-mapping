from networkx import DiGraph

from qft.common import QubitOp
from qft.deps import Dependency

from .interfaces import Scheduler


class RandomScheduler(Scheduler):
    def __init__(self, dep: Dependency) -> None:
        self.dep = dep
        self.consumer = self.dep.consumer()

    @property
    def dep_graph(self) -> DiGraph:
        return self.dep.g

    def next_op(self) -> QubitOp:
        ready = self.consumer.ready
        op = ready.pop()
        self.consumer.process(op)
        return op

    @property
    def done(self) -> bool:
        return self.consumer.terminate
