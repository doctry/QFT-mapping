#include "algo.hpp"
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
    if (conf["algo"].type() == json::value_t::null) {
        std::cerr << "Necessary key \"algo\" does not exist." << std::endl;
        abort();
    } else if (conf["algo"].type() == json::value_t::number_unsigned) {
        unsigned num_qubit = json_get<unsigned>(conf, "algo");
        topo = std::make_unique<topo::QFTTopology>(num_qubit);
    } else {
        std::fstream algo_file;
        auto algo_filename = json_get<std::string>(conf, "algo");
        std::cout << algo_filename << std::endl;
        algo_file.open(algo_filename, std::fstream::in);
        if (!algo_file.is_open()) {
            std::cerr << "There is no file" << algo_filename << std::endl;
            return 1;
        }
        topo = std::make_unique<topo::AlgoTopology>(algo_file);
    }

    // create device
    std::cout << "creating device..." << std::endl;
    json cycle_conf = json_get<json>(conf, "cycle");
    unsigned SINGLE_CYCLE = json_get<unsigned>(cycle_conf, "SINGLE_CYCLE");
    unsigned SWAP_CYCLE = json_get<unsigned>(cycle_conf, "SWAP_CYCLE");
    unsigned CX_CYCLE = json_get<unsigned>(cycle_conf, "CX_CYCLE");
    std::fstream device_file;
    std::string device_filename = json_get<std::string>(conf, "device");
    device_file.open(device_filename, std::fstream::in);
    if (!device_file.is_open()) {
        std::cerr << "There is no file" << device_filename << std::endl;
        return 1;
    }
    device::Device device(device_file, SINGLE_CYCLE, SWAP_CYCLE, CX_CYCLE);

    if (topo->get_num_qubits() > device.get_num_qubits()) {
        std::cerr << "You cannot assign more QFT qubits than the device."
                  << std::endl;
        return 1;
    }

    // create mapper
    json conf_mapper = json_get<json>(conf, "mapper");

    // place
    std::cout << "creating placer..." << std::endl;
    QFTPlacer placer;
    std::string placer_typ = json_get<std::string>(conf_mapper, "placer");
    std::vector<unsigned> assign = placer.place(device, placer_typ);
    device.place(assign);

    // scheduler
    json greedy_conf = json_get<json>(conf_mapper, "greedy_conf");
    std::cout << "creating scheduler..." << std::endl;
    std::string scheduler_typ =
        json_get<std::string>(conf_mapper, "scheduler");
    std::unique_ptr<QFTScheduler> scheduler = get_scheduler(scheduler_typ, *topo, greedy_conf);

    // router
    std::cout << "creating router..." << std::endl;
    std::string router_typ = json_get<std::string>(conf_mapper, "router");
    std::string cost = (scheduler_typ == "greedy")
                           ? json_get<std::string>(conf_mapper, "cost")
                           : "start";
    QFTRouter router(device, router_typ, cost);

    // routing
    std::cout << "routing..." << std::endl;
    scheduler->assign_gates(router);

    // dump
    bool dump = json_get<bool>(conf, "dump");
    if (dump) {
        std::cout << "dumping..." << std::endl;
        std::fstream out_file;
        out_file.open(json_get<std::string>(conf, "output"),
                      std::fstream::out);
        // out_file << assign << "\n";
        // device.write_assembly(out_file);
        json jj;
        jj["initial"] = assign;
        scheduler->to_json(jj);
        jj["final_cost"] = scheduler->get_final_cost();
        out_file << jj;
        // out_file << "final_cost: " << device.get_final_cost() << "\n";
    }

    if (json_get<bool>(conf, "stdio")) {
        scheduler->write_assembly(std::cout);
    }
    
    std::cout << "final cost:  " << scheduler->get_final_cost() << "\n";
    std::cout << "total time:  " << scheduler->get_total_time() << "\n";
    std::cout << "total swaps: " << scheduler->get_swap_num() << "\n";
    return 0;
}
