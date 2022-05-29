#include "algo.hpp"
#include "json.hpp"
#include "q_device.hpp"
#include "qft_mapper.hpp"
#include "qft_topo.hpp"
#include "topo.hpp"
#include "util.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./qft_mapping <config.json>";
        return 0;
    }

    // config file
    std::ifstream ifs(argv[1]);
    json conf = json::parse(ifs);

    // create topology
    std::cout << "creating topology..." << std::endl;
    std::unique_ptr<topo::Topology> topo;
    if (conf["algo"].type() == json::value_t::number_unsigned) {
        unsigned num_qubit = conf["algo"].get<unsigned>();
        topo =
            std::unique_ptr<topo::Topology>(new topo::QFTTopology(num_qubit));
    } else {
        std::fstream algo_file;
        std::cout << conf["algo"] << std::endl;
        algo_file.open(conf["algo"], std::fstream::in);
        if (!algo_file.is_open()) {
            std::cerr << "There is no file" << conf["algo"] << std::endl;
            return 1;
        }
        topo =
            std::unique_ptr<topo::Topology>(new topo::AlgoTopology(algo_file));
    }

    // create device
    std::cout << "creating device..." << std::endl;
    json cycle_conf = conf["cycle"].get<json>();
    unsigned R_CYCLE = cycle_conf["R_CYCLE"].get<unsigned>();
    unsigned SWAP_CYCLE = cycle_conf["SWAP_CYCLE"].get<unsigned>();
    unsigned CX_CYCLE = cycle_conf["CX_CYCLE"].get<unsigned>();
    std::fstream device_file;
    device_file.open(conf["device"], std::fstream::in);
    if (!device_file.is_open()) {
        std::cerr << "There is no file" << conf["device"] << std::endl;
        return 1;
    }
    device::Device device(device_file, R_CYCLE, SWAP_CYCLE, CX_CYCLE);

    if (topo->get_num_qubits() > device.get_num_qubits()) {
        std::cerr << "You cannot assign more QFT qubits than the device."
                  << std::endl;
        return 1;
    }

    // create mapper
    json conf_mapper = conf["mapper"].get<json>();

    // place
    std::cout << "creating placer..." << std::endl;
    QFTPlacer placer;
    std::string placer_typ = conf_mapper["placer"].get<std::string>();
    std::vector<unsigned> assign = placer.place(device, placer_typ);
    device.place(assign);

    // scheduler
    std::cout << "creating scheduler..." << std::endl;
    QFTScheduler scheduler(*topo);
    std::string scheduler_typ = conf_mapper["scheduler"].get<std::string>();

    // router
    std::cout << "creating router..." << std::endl;
    std::string router_typ = conf_mapper["router"].get<std::string>();
    std::string cost = (scheduler_typ == "greedy")
                           ? conf_mapper["cost"].get<std::string>()
                           : "start";
    QFTRouter router(device, router_typ, cost);

    // routing
    std::cout << "routing..." << std::endl;
    scheduler.assign_gates(device, router, scheduler_typ);

    // dump
    bool dump = conf["dump"].get<bool>();
    if (dump) {
        std::cout << "dumping..." << std::endl;
        std::fstream out_file;
        out_file.open(conf["output"], std::fstream::out);
        // out_file << assign << "\n";
        // device.write_assembly(out_file);
        json jj;
        jj["initial"] = assign;
        device.to_json(jj);
        jj["final_cost"] = device.get_final_cost();
        out_file << jj;
        // out_file << "final_cost: " << device.get_final_cost() << "\n";
    }

    if (conf["stdio"].get<bool>()) {
        device.write_assembly(std::cout);
    }
    std::cout << "final cost: " << device.get_final_cost() << "\n";
    std::cout << "total time: " << device.get_total_time() << "\n";
    return 0;
}