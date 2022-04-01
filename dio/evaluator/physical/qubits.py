class Qubit:
    def __init__(self, id: int, adj_list: list[int]):
        assert id > -1, id
        self.id = id
        self.adj_list = adj_list
        self.occupied_until: int = 0
        self.topo = id

        for i in self.adj_list:
            assert i > -1, self.adj_list

    def is_adj(self, other: int) -> bool:
        assert other > -1
        return other in self.adj_list

    def no_conflict(self, time: int) -> bool:
        return self.occupied_until < time

    def to_json(self) -> dict:
        return {"id": self.id, "adj_list": self.adj_list}

    def __str__(self):
        return "id: {}, topo_qubit: {}, occupied_until: {}".format(
            self.id, self.topo, self.occupied_until
        )
