import json
from pathlib import Path
from hydra import main
from omegaconf import DictConfig
import rich
from . import DeviceGraph


@main(config_path="conf", config_name="qft")
def app(cfg: DictConfig):
    proj_root = Path(cfg["proj_root"])
    device_path = Path(cfg["device"])

    with open(proj_root / device_path) as f:
        device = json.load(f)
        rich.print(device)
        device = DeviceGraph(device)
        rich.print(device, device.nodes, device.edges)


if __name__ == "__main__":
    app()
