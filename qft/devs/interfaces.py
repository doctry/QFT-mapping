import copy
from abc import abstractmethod
from typing import List, Protocol

from networkx import Graph


class Device(Protocol):
    @property
    def qubits(self) -> List[int]:
        return self.mapping()

    @abstractmethod
    def mapping(self) -> List[int]:
        ...

    @property
    @abstractmethod
    def g(self) -> Graph:
        ...

    def rotate(
        self, indices: List[int], *, right: bool, already_physical: bool
    ) -> None:
        rotated = self._rotate(
            indices, right=right, convert_to_physical=already_physical
        )
        cloned_map = copy.copy(self.qubits)

        for (idx, ridx) in zip(indices, rotated):
            self.qubits[idx] = cloned_map[ridx]

    def _rotate(
        self, indices: List[int], *, right: bool, convert_to_physical: bool
    ) -> List[int]:
        indices = self.to_physical(indices, convert_to_physical=convert_to_physical)

        if right:
            return indices[-1:] + indices[:-1]
        else:
            return indices[1:] + indices[:1]

    def to_physical(self, route: List[int], convert_to_physical: bool) -> List[int]:
        if not convert_to_physical:
            return route

        projector = self.device.qubits
        reverse = {q: idx for (idx, q) in enumerate(projector)}

        return [reverse[i] for i in route]
