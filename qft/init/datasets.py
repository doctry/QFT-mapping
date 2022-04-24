import math
from typing import Any, Dict

import h5py
import numpy as np
from torch.utils.data import Dataset, IterableDataset, get_worker_info


class FixedLenDataGen(Dataset):
    """
    This should be implemented by @doctry.
    """

    def __init__(self, h5_file: str) -> None:
        super().__init__()
        with h5py.File(h5_file, "r") as f:
            print(list(f.keys()))
            self.data = f["all_data"]
            self.data = np.array(self.data)

    def __len__(self) -> int:
        return len(self.data)

    def __getitem__(self, index: Any) -> Dict:
        return {"data": self.data[index, :-1], "label": self.data[index, -1]}


class DataGen(IterableDataset):
    """
    This should be implemented by @doctry.
    """

    def __init__(self, h5_file: str) -> None:
        super().__init__()
        self.data = FixedLenDataGen(h5_file)

    def __iter__(self):
        worker_info = get_worker_info()

        if worker_info is None:  # single-process data loading, return the full iterator
            iter_start = 0
            iter_end = len(self.data)
        else:  # in a worker process
            # split workload
            per_worker = int(math.ceil(len(self.data) / float(worker_info.num_workers)))
            worker_id = worker_info.id
            iter_start = worker_id * per_worker
            iter_end = min(iter_start + per_worker, len(self.data))
        return iter(self.data[iter_start:iter_end])


if __name__ == "__main__":
    data = FixedLenDataGen("../../data/guadalupe_16_1000000.hdf5")
    print(data[89])
