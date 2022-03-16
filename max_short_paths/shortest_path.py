from numpy import ndarray
from scipy.sparse import csgraph


def shortest_path(graph: ndarray) -> ndarray:
    return csgraph.shortest_path(graph)
