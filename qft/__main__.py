from __future__ import annotations

from pathlib import Path
from typing import Any, Mapping

from hydra import main

from qft import common

from . import (
    APSPScheduler,
    BaselineScheduler,
    Dependency,
    Device,
    PhysicalDevice,
    QFTDependency,
    SynchronousScheduler,
    Timing,
)


@main(config_path="conf", config_name="qft")
def run(cfg: Mapping[str, Any]) -> None:
    proj_root = Path(cfg["proj_root"])
    input_path = Path(cfg["input_path"])
    output_path = Path(cfg["output_path"])

    dev: Device = PhysicalDevice.from_file(proj_root / input_path)
    dep: Dependency = QFTDependency(len(dev.g.nodes))

    timing = Timing(cfg["time"]["swap"], cfg["time"]["op"])
    scheduler: APSPScheduler | None = None

    if (sch_typ := cfg["scheduler"]) == "baseline":
        scheduler = BaselineScheduler(dep, dev, timing)
    elif sch_typ == "sync":
        scheduler = SynchronousScheduler(dep, dev, timing)
    else:
        raise ValueError(f"Scheduler type: {sch_typ} not supported.")

    program = scheduler.schedule()
    common.write_json(program.json(), proj_root / output_path)


if __name__ == "__main__":
    run()
