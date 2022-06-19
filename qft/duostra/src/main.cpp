#include "algo.hpp"
#include "nlohmann/json.hpp"
#include "q_device.hpp"
#include "qft_mapper.hpp"
#include "qft_topo.hpp"
#include "topo.hpp"
#include "util.hpp"
#include <iostream>

template <class T> T get_wrapper(json &j, const char* key) {
    if (!j.contains(key)) {
        std::cerr << "Necessary key \"" << key << "\" does not exist."
                  << std::endl;
        abort();
    }
    return j[key].get<T>();
}

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
    }
    else if (conf["algo"].type() == json::value_t::number_unsigned) {
        unsigned num_qubit = get_wrapper<unsigned>(conf, "algo");
        topo = std::make_unique<topo::QFTTopology>(num_qubit);
    } else {
        std::fstream algo_file;
        std::string algo_filename = get_wrapper<std::string>(conf, "algo");
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
    json cycle_conf = get_wrapper<json>(conf, "cycle");
    unsigned SINGLE_CYCLE = get_wrapper<unsigned>(cycle_conf, "SINGLE_CYCLE");
    unsigned SWAP_CYCLE = get_wrapper<unsigned>(cycle_conf, "SWAP_CYCLE");
    unsigned CX_CYCLE = get_wrapper<unsigned>(cycle_conf, "CX_CYCLE");
    std::fstream device_file;
    std::string device_filename = get_wrapper<std::string>(conf, "device");
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
    json conf_mapper = get_wrapper<json>(conf, "mapper");

    // place
    std::cout << "creating placer..." << std::endl;
    QFTPlacer placer;
    std::string placer_typ = get_wrapper<std::string>(conf_mapper, "placer");
    std::vector<unsigned> assign = placer.place(device, placer_typ);
    device.place(assign);

    // scheduler
    int candidates_conf = get_wrapper<int>(conf_mapper, "greedy_candidates");
    unsigned candidates = UINT_MAX;
    if (candidates_conf > 0) {
        candidates = unsigned(candidates_conf);
    }
    std::cout << "creating scheduler..." << std::endl;
    QFTScheduler scheduler(*topo, candidates);
    std::string scheduler_typ = get_wrapper<std::string>(conf_mapper, "scheduler");

    // router
    std::cout << "creating router..." << std::endl;
    std::string router_typ = get_wrapper<std::string>(conf_mapper, "router");
    std::string cost = (scheduler_typ == "greedy")
                           ? get_wrapper<std::string>(conf_mapper, "cost")
                           : "start";
    QFTRouter router(device, router_typ, cost);

    // routing
    std::cout << "routing..." << std::endl;
    scheduler.assign_gates(router, scheduler_typ);

    // dump
    bool dump = get_wrapper<bool>(conf, "dump");
    if (dump) {
        std::cout << "dumping..." << std::endl;
        std::fstream out_file;
        out_file.open(get_wrapper<std::string>(conf, "output"),
                      std::fstream::out);
        // out_file << assign << "\n";
        // device.write_assembly(out_file);
        json jj;
        jj["initial"] = assign;
        scheduler.to_json(jj);
        jj["final_cost"] = scheduler.get_final_cost();
        out_file << jj;
        // out_file << "final_cost: " << device.get_final_cost() << "\n";
    }

    if (get_wrapper<bool>(conf, "stdio")) {
        scheduler.write_assembly(std::cout);
    }
    std::cout << "final cost: " << scheduler.get_final_cost() << "\n";
    std::cout << "total time: " << scheduler.get_total_time() << "\n";
    return 0;
}
