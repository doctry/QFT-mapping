from . import DependencyGraph, JsonSerDe, OperationEdge


def test_json():
    def jsonserde(js: JsonSerDe):
        js.to_json()

    dep = DependencyGraph(5)
    jsonserde(dep)


def test_dep_1():
    dep = DependencyGraph(1).to_json()

    assert dep["nodes"] == []
    assert dep["edges"] == []


def test_dep_2():
    dep = DependencyGraph(2).to_json()

    assert dep["nodes"] == [[0, 1]]
    assert dep["edges"] == []


def test_dep_3():
    dep = DependencyGraph(3).to_json()

    assert sorted(dep["nodes"]) == sorted([[0, 1], [0, 2], [1, 2]])
    assert sorted(dep["edges"]) == sorted(
        [
            [[0, 1], [0, 2]],
            [[0, 2], [1, 2]],
        ]
    )


def test_dep_4():
    dep = DependencyGraph(4).to_json()

    assert sorted(dep["nodes"]) == sorted(
        [[0, 1], [0, 2], [1, 2], [0, 3], [1, 3], [2, 3]]
    )
    assert sorted(dep["edges"]) == sorted(
        [
            [[0, 1], [0, 2]],
            [[0, 2], [1, 2]],
            [[0, 2], [0, 3]],
            [[0, 3], [1, 3]],
            [[1, 2], [1, 3]],
            [[1, 3], [2, 3]],
        ]
    )
