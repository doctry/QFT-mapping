from email.generator import Generator
from typing import Any, Iterable

from alive_progress import alive_bar


def progress(iterable: Iterable[Any]) -> Generator[Any, None, None]:
    with alive_bar(len(iterable)) as bar:
        for it in iterable:
            yield it
            bar()
