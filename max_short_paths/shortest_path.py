from scipy.sparse import csgraph
from numpy import ndarray


def shortest_path(graph: ndarray) -> ndarray:
    return csgraph.shortest_path(graph)
