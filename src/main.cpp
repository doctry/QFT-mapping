#include <iostream>
#include <device.h>
#include <qft_topo.h>

int main (int argc, char *argv[]) {
    if(argc != 3) {
        std::cerr << "Usage: ./qft_mapping <num_qubit> <device_file>\n"; 
        return 0;
    }

    unsigned num_qubit = std::stoi(argv[1]);

    topo::QFTTopology qft_topo(num_qubit);

}