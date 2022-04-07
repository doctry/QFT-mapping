from typing import Any, Callable, Dict, Generic, TypeVar

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
    def __init__(self) -> None:
        self._reverse = Dict[V, K]()

        def _mapping_setitem_callback(k: K, v: V) -> None:
            self._reverse[v] = k

        self._mapping = CallbackDict[K, V](setitem=_mapping_setitem_callback)

    @property
    def mapping(self) -> Dict[K, V]:
        return self._mapping

    @property
    def reverse(self) -> Dict[V, K]:
        return self._reverse
