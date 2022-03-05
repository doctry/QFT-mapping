#include <iostream>
#include "device.h"
#include "qft_topo.h"
#include "qft_mapper.h"

int main (int argc, char *argv[]) {
    if(argc != 3) {
        std::cerr << "Usage: ./qft_mapping <num_qubit> <device_file>\n"; 
        return 0;
    }

    unsigned num_qubit = std::stoi(argv[1]);
    topo::QFTTopology qft_topo(num_qubit);

    std::fstream file;
    file.open(argv[2], std::fstream::in);

    device::Device device(file);

    if(num_qubit < device.get_num_qubits()) {
        std::cerr << "You cannot assign more QFT qubits than the device.";
    }

    QFTMapper mapper(qft_topo, device);
    mapper.init_place();
    mapper.assign_gates();
    
    device.print_operations(std::cout);

    return 0;
}