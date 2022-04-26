from __future__ import annotations

from typing import Any, Generator, Iterable, Sequence

from alive_progress import alive_bar


def progress(iterable: Sequence[Any] | int) -> Generator[Any, None, None]:
    if isinstance(iterable, int):
        length = iterable
        iterable = range(length)
    else:
        length = len(iterable)

    with alive_bar(length) as bar:
        for it in iterable:
            yield it
            bar()
