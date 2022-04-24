import json
import sys

import duostra
import h5py
import numpy as np


def main():
    assert len(sys.argv) == 2

    with open(sys.argv[1], "r") as f:
        conf = json.load(f)

    data = []
    for i in range(conf["gen_data"]):
        num_qubit = conf["num_qubits"]
        R_CYCLE = conf["R_CYCLE"]
        SWAP_CYCLE = conf["SWAP_CYCLE"]
        qft_topo = duostra.QFTTopologyCpp(num_qubit)

        with open(conf["device"], "r") as f:
            device_file = json.load(f)
        device_file = [i["adj_list"] for i in device_file]
        device = duostra.DeviceCpp(device_file, R_CYCLE, SWAP_CYCLE)

        placer = duostra.QFTPlacerCpp()

        # device2topo
        shuffle = placer.place(device, conf["random_place"])

        scheduler = duostra.QFTSchedulerCpp(qft_topo)
        router = duostra.QFTRouterCpp(device)
        scheduler.assign_gates(device, router)

        print(f"iter {i}, final cost: {device.get_final_cost()}")
        shuffle.append(device.get_final_cost())
        data.append(shuffle)

    with h5py.File(conf["h5"], "w") as f:
        dset = f.create_dataset("all_data", data=np.array(data))


if __name__ == "__main__":
    main()
