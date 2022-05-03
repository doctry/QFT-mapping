import copy
from abc import abstractmethod
from typing import Dict, List, Mapping, Protocol

from networkx import Graph


class Device(Protocol):
    @property
    def qubits(self) -> Dict[int, int]:
        return self.mapping()

    @abstractmethod
    def mapping(self) -> Dict[int, int]:
        ...

    @property
    @abstractmethod
    def reverse(self) -> Dict[int, int]:
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
        indices = [
            self.to_physical(i, convert_to_physical=convert_to_physical)
            for i in indices
        ]

        if right:
            return indices[-1:] + indices[:-1]
        else:
            return indices[1:] + indices[:1]

    def to_physical(self, qubit: int, convert_to_physical: bool) -> int:
        if not convert_to_physical:
            return qubit
        return self.reverse[qubit]
