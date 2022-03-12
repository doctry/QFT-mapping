#include <iostream>
#include "device.h"
#include "qft_topo.h"
#include "qft_mapper.h"
#include "args.h"

Args args;

int main (int argc, char *argv[]) {
    if(argc != 5) {
        std::cerr << "Usage: ./qft_mapping <num_qubit> <device_file> <R_cycle> <swap_cycle>\n"; 
        return 0;
    }

    unsigned num_qubit = std::stoi(argv[1]);
    args.R_CYCLE = std::stoi(argv[3]);
    args.SWAP_CYCLE = std::stoi(argv[4]);
    topo::QFTTopology qft_topo(num_qubit);

    std::fstream file;
    file.open(argv[2], std::fstream::in);
    if (!file.is_open()) {
        std::cerr << "There is no file" << argv[2] << std::endl;
        return 1;
    }

    device::Device device(file);

    if(num_qubit > device.get_num_qubits()) {
        std::cerr << "You cannot assign more QFT qubits than the device." << std::endl;
        return 1;
    }

    QFTMapper mapper(qft_topo, device);
    mapper.init_place();
    mapper.assign_gates();
    
    device.print_operations(std::cout);
    std::cout << "final cost: " << device.get_final_cost() << "\n";

    return 0;
}