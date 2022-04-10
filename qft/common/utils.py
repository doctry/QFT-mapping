from typing import Any, Dict


def union_if(dictionary: Dict[str, Any], **kwargs: Any) -> None:
    for (key, value) in kwargs.items():
        if value is not None:
            dictionary |= {key: value}
    return
