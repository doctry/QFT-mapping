import json

import qft.duostra as ds
from qft.deps import QFTDependency

from .interfaces import Scheduler


class DuostraScheduler(Scheduler):
    def __init__(self, cfg, scheduler: bool = True):
        with open(cfg["device"], "r") as f:
            device_file = json.load(f)
        device_file = [i["adj_list"] for i in device_file]
        device_cpp = ds.DeviceCpp(device_file, cfg["time"]["op"], cfg["time"]["swap"])
        device = None

        qft_topo = QFTDependency(len(device_file))

    with open(conf["device"], "r") as f:
        device_file = json.load(f)
    device_file = [i["adj_list"] for i in device_file]
    device = duostra.DeviceCpp(device_file, R_CYCLE, SWAP_CYCLE)

    placer = duostra.QFTPlacerCpp()
    placer.place(device, conf["random_place"])

    scheduler = duostra.QFTSchedulerCpp(qft_topo)
    router = duostra.QFTRouterCpp(device)
    scheduler.assign_gates(device, router)

    print("final cost: ", device.get_final_cost())

    def schedule(self):
        pass
