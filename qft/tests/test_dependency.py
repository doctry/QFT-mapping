from qft import Dependency, QFTDependency


def test_dep() -> None:
    def function(dep: Dependency) -> None:
        dep.dependency
        dep.consumer
        dep.json()

    function(QFTDependency(2))


# def test_dep_1() -> None:
#     dep = QFTDependency(1).json()

#     assert dep["nodes"] == []
#     assert dep["edges"] == []


# def test_dep_2() -> None:
#     dep = QFTDependency(2).json()

#     assert dep["nodes"] == [[0, 1]]
#     assert dep["edges"] == []


# def test_dep_3() -> None:
#     dep = QFTDependency(3).json()

#     assert sorted(dep["nodes"]) == sorted([[0, 1], [0, 2], [1, 2]])
#     assert sorted(dep["edges"]) == sorted(
#         [
#             [[0, 1], [0, 2]],
#             [[0, 2], [1, 2]],
#         ]
#     )


# def test_dep_4() -> None:
#     dep = QFTDependency(4).json()

#     assert sorted(dep["nodes"]) == sorted(
#         [[0, 1], [0, 2], [1, 2], [0, 3], [1, 3], [2, 3]]
#     )
#     assert sorted(dep["edges"]) == sorted(
#         [
#             [[0, 1], [0, 2]],
#             [[0, 2], [1, 2]],
#             [[0, 2], [0, 3]],
#             [[0, 3], [1, 3]],
#             [[1, 2], [1, 3]],
#             [[1, 3], [2, 3]],
#         ]
#     )
