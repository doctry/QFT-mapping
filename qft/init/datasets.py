from typing import Any

from torch.utils.data import Dataset, IterableDataset


class FixedLenDataGen(Dataset):
    """
    This should be implemented by @doctry.
    """

    def __init__(self) -> None:
        super().__init__()

    def __len__(self) -> int:
        raise NotImplementedError

    def __getitem__(self, index: Any) -> Any:
        raise NotImplementedError


class DataGen(IterableDataset):
    """
    This should be implemented by @doctry.
    """

    def __init__(self) -> None:
        super().__init__()

    def __iter__(self):
        raise NotImplementedError
