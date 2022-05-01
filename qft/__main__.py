# pyright: reportGeneralTypeIssues=false
from __future__ import annotations

import random
import sys
from typing import Any, Mapping

import networkx as nx
from black import json
from hydra import main, utils
from loguru import logger

from qft.controllers.apsp import APSPController

from . import (
    APSPRouter,
    Dependency,
    Device,
    DuostraController,
    DuostraRouter,
    PhysicalDevice,
    QFTDependency,
    RandomScheduler,
    Timing,
    duostra,
    write_json,
)


@main(config_path="conf", config_name="qft")
def run(cfg: Mapping[str, Any]) -> None:
    logger.remove()
    logger.add(sys.stderr, level=cfg["level"])
    device_path = str(cfg["device"])
    result_path = cfg["result"]

    timing = Timing(cfg["time"]["swap"], cfg["time"]["op"])

    # scheduler
    dev: Device = PhysicalDevice.from_file(utils.to_absolute_path(device_path))

    if cfg["components"]["dependency"] == "qft":
        dep: Dependency = QFTDependency(len(dev.g.nodes))
    else:
        raise ValueError

    if cfg["components"]["scheduler"] == "random":
        scheduler = RandomScheduler(dep)
    else:
        raise ValueError

    if cfg["components"]["device"] == "duostra":
        placer = duostra.QFTPlacerCpp()

        # device2topo
        with open(cfg["device"], "r") as f:
            device_file = json.load(f)
        device_file = [i["adj_list"] for i in device_file]
        device = duostra.DeviceCpp(device_file, cfg["time"]["op"], cfg["time"]["swap"])
        placer.place(device, False)
    elif cfg["components"]["device"] == "apsp":
        with open(cfg["device"], "r") as f:
            device_file = json.load(f)
        device = PhysicalDevice(device_file)
        random.shuffle(device.qubits)
        zero = device.qubits.index(0)
        path_from_0 = tuple(nx.shortest_path(device.g)[zero])
        assert len(path_from_0) == len(device.g.ndoes)
        path_idx_from_0 = tuple((p, i) for (i, p) in enumerate(path_from_0))
        shortest_path_idx = sorted(path_idx_from_0)
        shortest_idx = tuple(x[1] for x in shortest_path_idx)
        device.qubits[:] = shortest_idx
    else:
        raise ValueError

    if cfg["components"]["router"] == "duostra":
        router = DuostraRouter(device)
    elif cfg["components"]["router"] == "apsp":
        router = APSPRouter(device)
    else:
        raise ValueError

    if cfg["components"]["controller"] == "duostra":
        controller = DuostraController(device, router, scheduler)
    elif cfg["components"]["controller"] == "apsp":
        controller = APSPController(device, scheduler, router, timing)
    else:
        raise ValueError

    result = controller.compile()

    if result_path is not None:
        write_json(result.json(), result_path)

    print(f"final_cost: {result.cost}")


if __name__ == "__main__":
    run()
