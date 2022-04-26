from __future__ import annotations

from pathlib import Path
from typing import Any, Mapping

from hydra import main, utils

from qft import common

from . import Dependency, Device, PhysicalDevice, QFTDependency, Timing


@main(config_path="conf", config_name="qft")
def run(cfg: Mapping[str, Any]) -> None:
    device_path = str(cfg["device"])
    result_path = str(cfg["result"])

    dev: Device = PhysicalDevice.from_file(utils.to_absolute_path(device_path))
    dep: Dependency = QFTDependency(len(dev.g.nodes))

    timing = Timing(cfg["time"]["swap"], cfg["time"]["op"])
    # scheduler: APSPScheduler | None = None

    # if (sch_typ := cfg["scheduler"]) == "baseline":
    #     scheduler = BaselineScheduler(cfg, dep, dev, timing)
    # elif sch_typ == "sync":
    #     scheduler = SynchronousScheduler(cfg, dep, dev, timing)
    # else:
    #     raise ValueError(f"Scheduler type: {sch_typ} not supported.")

    # program = scheduler.schedule()
    # common.write_json(program.json(), result_path)


if __name__ == "__main__":
    run()
