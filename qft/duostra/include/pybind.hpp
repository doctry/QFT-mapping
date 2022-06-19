#pragma once

#include "algo.hpp"
#include "q_device.hpp"
#include "qft_mapper.hpp"
#include "qft_topo.hpp"
#include "topo.hpp"
#include "util.hpp"
#include <iostream>

class Configs {
  public:
    class CycleConfigs {
      public:
        unsigned SINGLE_CYCLE, CX_CYCLE, SWAP_CYCLE;
    };
    class MapperConfigs {
      public:
        unsigned candidates;
        std::string placer, scheduler, router;
        std::string cost;
    };
    Configs() : algo_num(0) {}

    // member
    unsigned algo_num;
    std::string algo_filename, device, output;
    MapperConfigs mapper_conf;
    CycleConfigs cycle_conf;
    bool dump, stdio;
};

int main_py(Configs &conf) {
    // create topology
    std::cout << "creating topology..." << std::endl;
    std::unique_ptr<topo::Topology> topo;
    if (conf.algo_num > 0) {
        topo = std::make_unique<topo::QFTTopology>(conf.algo_num);
    } else {
        std::fstream algo_file;
        std::cout << conf.algo_filename << std::endl;
        algo_file.open(conf.algo_filename, std::fstream::in);
        if (!algo_file.is_open()) {
            std::cerr << "There is no file" << conf.algo_filename << std::endl;
            return 1;
        }
        topo = std::make_unique<topo::AlgoTopology>(algo_file);
    }

    // create device
    std::cout << "creating device..." << std::endl;
    auto &cycle_conf = conf.cycle_conf;
    std::fstream device_file;
    device_file.open(conf.device, std::fstream::in);
    if (!device_file.is_open()) {
        std::cerr << "There is no file" << conf.device << std::endl;
        return 1;
    }
    device::Device device(device_file, cycle_conf.SINGLE_CYCLE,
                          cycle_conf.SWAP_CYCLE, cycle_conf.CX_CYCLE);

    if (topo->get_num_qubits() > device.get_num_qubits()) {
        std::cerr << "You cannot assign more QFT qubits than the device."
                  << std::endl;
        return 1;
    }

    // create mapper
    auto &mapper_conf = conf.mapper_conf;

    // place
    std::cout << "creating placer..." << std::endl;
    QFTPlacer placer;
    std::vector<unsigned> assign = placer.place(device, mapper_conf.placer);
    device.place(assign);

    // scheduler
    int candidates_conf = mapper_conf.candidates;
    unsigned candidates = UINT_MAX;
    if (candidates_conf > 0) {
        candidates = unsigned(candidates_conf);
    }
    std::cout << "creating scheduler..." << std::endl;
    QFTScheduler scheduler(*topo, candidates);

    // router
    std::cout << "creating router..." << std::endl;
    std::string cost =
        (mapper_conf.scheduler == "greedy") ? mapper_conf.cost : "start";
    QFTRouter router(device, mapper_conf.router, cost);

    // routing
    std::cout << "routing..." << std::endl;
    scheduler.assign_gates(router, mapper_conf.scheduler);

    // dump
    if (conf.dump) {
        std::cout << "dumping..." << std::endl;
        std::fstream out_file;
        out_file.open(conf.output, std::fstream::out);
        // out_file << assign << "\n";
        // device.write_assembly(out_file);
        json jj;
        jj["initial"] = assign;
        scheduler.to_json(jj);
        jj["final_cost"] = scheduler.get_final_cost();
        out_file << jj;
        // out_file << "final_cost: " << device.get_final_cost() << "\n";
    }

    if (conf.stdio) {
        scheduler.write_assembly(std::cout);
    }
    std::cout << "final cost: " << scheduler.get_final_cost() << "\n";
    std::cout << "total time: " << scheduler.get_total_time() << "\n";
    return 0;
}
