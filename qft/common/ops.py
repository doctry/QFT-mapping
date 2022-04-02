from __future__ import annotations

from typing import Dict, NamedTuple, Sequence


class QubitOp(NamedTuple):
    name: str | None
    source: int
    target: int