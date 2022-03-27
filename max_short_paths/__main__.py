import sys
from pathlib import Path

import loguru
from hydra import main
from omegaconf import DictConfig

from . import DependencyGraph, DeviceDriver, DeviceGraph


@main(config_path="conf", config_name="qft")
def app(cfg: DictConfig):
    proj_root = Path(cfg["proj_root"])
    device_path = Path(cfg["device"])

    loguru.logger.remove()
    loguru.logger.add(sys.stderr, level=cfg["level"])

    device = DeviceGraph.from_file(proj_root / device_path)
    loguru.logger.debug(f"Device: {device.to_json()}")

    driver = DeviceDriver(device)
    loguru.logger.debug(f"Driver: {driver}")

    dep_graph = DependencyGraph(len(device.nodes))
    loguru.logger.debug(dep_graph.to_json())

    consumer = dep_graph.consumer
    loguru.logger.debug(consumer)


if __name__ == "__main__":
    app()
