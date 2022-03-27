from . import JsonSerDe, OperationEdge


def test_edge():
    e = OperationEdge(1, 2)
    tup = (1, 2)

    assert e == tup

    tup = (1, 2, 3)
    assert e != tup


def test_impl():
    def jsonserde(js: JsonSerDe):
        js.to_json()

    e = OperationEdge(0, 5)
    jsonserde(e)
