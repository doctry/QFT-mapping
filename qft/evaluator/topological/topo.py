from __future__ import annotations
from abc import ABC
from typing import Any, Dict, List, Tuple

class Gate:
    def __init__(self, id: int, typ: str, qubits: List[int], prev: List[Dict[str, Any]]):
        self.id = id
        self.typ = typ
        self.qubits = qubits
        self.prev: List[Dict[str, Any]] = []
        self.next: List[int] = []
        if prev[0]["id"] != -1:
            self.prev.append(prev[0])
        if prev[0]["id"] != prev[1]["id"]:
            if prev[1]["id"] != -1:
                self.prev.append(prev[1])

    def is_avail(self):
        avail = True
        for item in self.prev:
            if item["done"] == False:
                avail = False
                break
        return avail

    def finished(self, prevId: int):
        for item in self.prev:
            if item["id"] == prevId:
                item["done"] = True

    def add_next(self, id: int):
        if id != -1:
            self.next.append(id)

    def get_next(self):
        return self.next

class Topo(ABC):
    avail_gates: List[int]
    gates: List[Gate]

    def __init__(self):
        pass

    def update_avail_gates(self, executed: int) -> None:
        ...
