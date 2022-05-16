from typing import Dict, List

from .qubits import Qubit


class Device:
    def __init__(self, filename: str):
        nums = []
        self.qubits: list[Qubit] = []

        with open(filename, "r") as f:
            for x in f:
                x = x.split()
                for y in x:
                    nums.append(int(y))

        count = 0
        num = nums[0]
        for i in range(num):
            count += 1
            id = nums[count]
            assert i == id, (i, id)

            count += 1
            num_adj = nums[count]
            adj = []
            for _ in range(num_adj):
                count += 1
                adj.append(nums[count])
            self.qubits.append(Qubit(i, adj))

        assert len(self.qubits) == num

        for (i, qubit) in enumerate(self.qubits):
            for j in qubit.adj_list:
                assert i in self.qubits[j].adj_list, (i, j, self.qubits[j].adj_list)

    def to_json(self) -> List[Dict]:
        ret = []
        for q in self.qubits:
            ret.append(q.to_json())
        return ret

    def place(self, assign: list[int]):
        for (i, d) in enumerate(assign):
            self.qubits[d].topo = i

    def __str__(self):
        ret = ""
        for q in self.qubits:
            ret += q.__str__() + "\n"
        return ret
