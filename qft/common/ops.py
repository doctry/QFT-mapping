from __future__ import annotations

from typing import NamedTuple


class QubitOp(NamedTuple):
    name: str | None
    source: int
    target: int
