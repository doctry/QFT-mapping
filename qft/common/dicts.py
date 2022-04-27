from typing import Any, Callable, Dict, Generic, Iterator, Mapping, TypeVar

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
        self._reverse = Dict[V, K]()

        def _mapping_setitem_callback(k: K, v: V) -> None:
            self._reverse[v] = k

        self._mapping = CallbackDict[K, V](setitem=_mapping_setitem_callback)

        for (key, val) in mapping.items():
            self._mapping[key] = val

            assert self.mapping[key] == val
            assert self.reverse[val] == key

    def __iter__(self) -> Iterator[K]:
        return iter(self.mapping)

    def keys(self) -> Iterator[K]:
        return self.mapping.keys()

    def values(self) -> Iterator[V]:
        return self.mapping.values()

    def items(self, reverse: bool = False):
        if reverse:
            return self.reverse.items()
        else:
            return self.mapping.items()

    def __getitem__(self, key: K) -> V:
        return self.mapping[key]

    def __setitem__(self, key: K, val: V) -> None:
        self.mapping[key] = val

    @property
    def mapping(self) -> Dict[K, V]:
        return self._mapping

    @property
    def reverse(self) -> Dict[V, K]:
        return self._reverse
