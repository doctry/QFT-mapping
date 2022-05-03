from typing import Any, Callable, Dict, Generic, Iterable, Iterator, Mapping, TypeVar

K = TypeVar("K")
V = TypeVar("V")


class CallbackDict(Dict[K, V]):
    def __init__(
        self,
        *,
        getitem: Callable[[K], None] = lambda k: None,
        setitem: Callable[[K, V], None] = lambda k, v: None,
        delitem: Callable[[K], None] = lambda k: None,
        **kwargs: Any
    ) -> None:
        super().__init__(**kwargs)

        self._getitem = getitem
        self._setitem = setitem
        self._delitem = delitem

    def __getitem__(self, k: K) -> V:
        item = super().__getitem__(k)
        self._getitem(k)
        return item

    def __setitem__(self, k: K, v: V) -> None:
        super().__setitem__(k, v)
        self._setitem(k, v)

    def __delitem__(self, k: K) -> None:
        super().__delitem__(k)
        self._delitem(k)


class TwoWayDict(Generic[K, V]):
    def __init__(self, mapping: Mapping[K, V]) -> None:
        self._reverse = {}

        def _mapping_setitem_callback(k: K, v: V) -> None:
            self._reverse[v] = k

        self._forward = CallbackDict(setitem=_mapping_setitem_callback)

        for (key, val) in mapping.items():
            self._forward[key] = val

            assert self._forward[key] == val
            assert self._reverse[val] == key

    def __iter__(self) -> Iterator[K]:
        return iter(self._forward)

    def keys(self) -> Iterable[K]:
        return self._forward.keys()

    def values(self) -> Iterable[V]:
        return self._forward.values()

    def items(self, reverse: bool = False):
        if reverse:
            return self.reverse.items()
        else:
            return self._forward.items()

    def __getitem__(self, key: K) -> V:
        return self._forward[key]

    def __setitem__(self, key: K, val: V) -> None:
        self._forward[key] = val

    @property
    def forward(self) -> Dict[K, V]:
        return self._forward

    @property
    def reverse(self) -> Dict[V, K]:
        return self._reverse
