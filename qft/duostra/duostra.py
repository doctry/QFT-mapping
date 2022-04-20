import duostra
import sys
import json

def main():
    assert len(sys.argv) == 2

    with open(sys.argv[1], 'r') as f:
        conf = json.load(f)

    num_qubit = conf["num_qubits"]
    R_CYCLE = conf["R_CYCLE"]
    SWAP_CYCLE = conf["SWAP_CYCLE"]
    qft_topo = duostra.QFTTopologyCpp(num_qubit)

    with open(conf["device"], 'r') as f:
        device_file = json.load(f)
    device_file = [i["adj_list"] for i in device_file]
    device = duostra.DeviceCpp(device_file, R_CYCLE, SWAP_CYCLE)

    placer = duostra.QFTPlacerCpp()
    placer.place(device, conf["random_place"])

    scheduler = duostra.QFTSchedulerCpp(qft_topo)
    router = duostra.QFTRouterCpp(device)
    scheduler.assign_gates(device, router)

    print("final cost: ", device.get_final_cost())



if __name__ == "__main__":
    main()