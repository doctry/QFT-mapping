import json
import sys
from pathlib import Path

import loguru
from hydra import main
from omegaconf import DictConfig

from . import DependencyGraph, DeviceGraph


@main(config_path="conf", config_name="qft")
def app(cfg: DictConfig):
    proj_root = Path(cfg["proj_root"])
    device_path = Path(cfg["device"])

    loguru.logger.remove()
    loguru.logger.add(sys.stderr, level=cfg["level"])

    with open(proj_root / device_path) as f:
        device = json.load(f)
        loguru.logger.debug(device)
        device = DeviceGraph(device)
        loguru.logger.debug(device)
        loguru.logger.debug(device.nodes)
        loguru.logger.debug(device.edges)

    dep_graph = DependencyGraph(len(device.nodes))
    loguru.logger.debug(dep_graph.to_json())

    consumer = dep_graph.consumer
    loguru.logger.debug(consumer)


if __name__ == "__main__":
    app()
