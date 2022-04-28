import copy
from abc import abstractmethod
from typing import List, Protocol

from networkx import Graph

from qft.common import TwoWayDict


class Device(Protocol):
    @abstractmethod
    def mapping(self) -> TwoWayDict[int, int]:
        ...

    @property
    @abstractmethod
    def g(self) -> Graph:
        ...

    def rotate(self, indices: List[int], *, right: bool) -> None:
        rotated = self._rotate(indices, right=right)
        cloned_map = copy.copy(self.mapping)

        for (idx, ridx) in zip(indices, rotated):
            self.mapping.mapping[idx] = cloned_map.mapping[ridx]

    @staticmethod
    def _rotate(indices: List[int], *, right: bool) -> List[int]:
        if right:
            return indices[-1:] + indices[:-1]
        else:
            return indices[1:] + indices[:1]
