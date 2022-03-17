from __future__ import annotations

from typing import Tuple

from networkx import Graph


class Consumer:
    def __init__(self, dependency: Graph) -> None:
        self._dep = dependency
        self._history = set()
        self._available = set()

    @property
    def dependency(self) -> Graph:
        return self._dep

    @property
    def history(self) -> Tuple[int]:
        return tuple(self._history)

    @property
    def available(self) -> Tuple[int]:
        return tuple(self._available)

    def enter(self, index: int) -> None:
        assert index in self._available, [index, self._available]

        adj = self._dep.adj[index]
        self._available.remove(index)
        self._available |= set(adj.keys())
