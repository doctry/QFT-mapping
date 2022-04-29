#include <iostream>
#include "device.h"
#include "qft_topo.h"
#include "qft_mapper.h"
#include "string"
#include "json.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./qft_mapping <config.json>";
        return 0;
    }

    std::ifstream ifs(argv[1]);
    json conf = json::parse(ifs);

    unsigned num_qubit = conf["num_qubits"].get<unsigned>();
    unsigned R_CYCLE = conf["R_CYCLE"].get<unsigned>();
    unsigned SWAP_CYCLE = conf["SWAP_CYCLE"].get<unsigned>();
    topo::QFTTopology qft_topo(num_qubit);

    std::fstream device_file;
    device_file.open(conf["device"], std::fstream::in);
    if (!device_file.is_open())
    {
        std::cerr << "There is no file" << conf["device"] << std::endl;
        return 1;
    }

    device::Device device(device_file, R_CYCLE, SWAP_CYCLE);

    if (num_qubit > device.get_num_qubits())
    {
        std::cerr << "You cannot assign more QFT qubits than the device." << std::endl;
        return 1;
    }

    QFTPlacer placer;
    placer.place(device, conf["random_place"].get<bool>());

    QFTScheduler scheduler(qft_topo);
    QFTRouter router(device);
    scheduler.assign_gates(device, router);

    std::fstream out_file;
    out_file.open(conf["output"], std::fstream::out);
    device.write_assembly(out_file);
    out_file << "final_cost: " << device.get_final_cost() << "\n";

    if (conf["stdio"].get<bool>())
    {
        device.write_assembly(std::cout);
    }
    std::cout << "final cost: " << device.get_final_cost() << "\n";
    std::cout << "total time: " << device.get_total_time() << "\n";
    return 0;
}