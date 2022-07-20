#include "flow.hpp"

#include "qft_mapper.hpp"

using namespace std;

unsigned flow(json& conf, vector<unsigned> assign, bool io) {
    if (!io) {
        cout.setstate(ios_base::failbit);
    }
    // create topology
    cout << "creating topology..." << endl;
    unique_ptr<topo::Topology> topo;
    if (conf["algo"].type() == json::value_t::null) {
        cerr << "Necessary key \"algo\" does not exist." << endl;
        abort();
    } else if (conf["algo"].type() == json::value_t::number_unsigned) {
        unsigned num_qubit = json_get<unsigned>(conf, "algo");
        topo = make_unique<topo::QFTTopology>(num_qubit);
    } else {
        fstream algo_file;
        auto algo_filename = json_get<string>(conf, "algo");
        cout << algo_filename << endl;
        algo_file.open(algo_filename, fstream::in);
        if (!algo_file.is_open()) {
            cerr << "There is no file" << algo_filename << endl;
            abort();
        }
        bool onlyIBM = json_get<unsigned>(conf, "IBM_Gate");
        topo = make_unique<topo::AlgoTopology>(algo_file, onlyIBM);
    }

    // create device
    cout << "creating device..." << endl;
    json cycle_conf = json_get<json>(conf, "cycle");
    unsigned SINGLE_CYCLE = json_get<unsigned>(cycle_conf, "SINGLE_CYCLE");
    unsigned SWAP_CYCLE = json_get<unsigned>(cycle_conf, "SWAP_CYCLE");
    unsigned CX_CYCLE = json_get<unsigned>(cycle_conf, "CX_CYCLE");
    fstream device_file;
    string device_filename = json_get<string>(conf, "device");
    device_file.open(device_filename, fstream::in);
    if (!device_file.is_open()) {
        cerr << "There is no file" << device_filename << endl;
        abort();
    }
    device::Device device(device_file, SINGLE_CYCLE, SWAP_CYCLE, CX_CYCLE);

    if (topo->get_num_qubits() > device.get_num_qubits()) {
        cerr << "You cannot assign more QFT qubits than the device." << endl;
        abort();
    }

    // create mapper
    json conf_mapper = json_get<json>(conf, "mapper");

    // place
    cout << "creating placer..." << endl;
    if (assign.size() == 0) {
        string placer_typ = json_get<string>(conf_mapper, "placer");
        QFTPlacer placer;
        assign = placer.place(device, placer_typ);
    }
    device.place(assign);

    // scheduler
    json greedy_conf = json_get<json>(conf_mapper, "greedy_conf");
    cout << "creating scheduler..." << endl;
    string scheduler_typ = json_get<string>(conf_mapper, "scheduler");
    auto sched = scheduler::get(scheduler_typ, move(topo), greedy_conf);

    // router
    cout << "creating router..." << endl;
    string router_typ = json_get<string>(conf_mapper, "router");
    bool orient = json_get<bool>(conf_mapper, "orientation");
    string cost = (scheduler_typ == "greedy" || scheduler_typ == "onion")
                      ? json_get<string>(conf_mapper, "cost")
                      : "start";
    auto router =
        make_unique<QFTRouter>(move(device), router_typ, cost, orient);

    // routing
    cout << "routing..." << endl;
    sched->assign_gates(move(router));
    sched->sort();

    // dump
    bool dump = json_get<bool>(conf, "dump");
    if (dump) {
        cout << "dumping..." << endl;
        fstream out_file;
        out_file.open(json_get<string>(conf, "output"), fstream::out);
        json jj;
        jj["initial"] = assign;
        sched->to_json(jj);
        jj["final_cost"] = sched->get_final_cost();
        out_file << jj;
    }

    if (json_get<bool>(conf, "stdio")) {
        sched->write_assembly(cout);
    }

    cout << "final cost: " << sched->get_final_cost() << "\n";
    cout << "total time: " << sched->get_total_time() << "\n";
    cout << "total swaps: " << sched->get_swap_num() << "\n";

    cout.clear();
    return sched->get_final_cost();
}

unsigned device_num(json& conf) {
    // create device
    cout << "creating device..." << endl;
    json cycle_conf = json_get<json>(conf, "cycle");
    unsigned SINGLE_CYCLE = json_get<unsigned>(cycle_conf, "SINGLE_CYCLE");
    unsigned SWAP_CYCLE = json_get<unsigned>(cycle_conf, "SWAP_CYCLE");
    unsigned CX_CYCLE = json_get<unsigned>(cycle_conf, "CX_CYCLE");
    fstream device_file;
    string device_filename = json_get<string>(conf, "device");
    device_file.open(device_filename, fstream::in);
    if (!device_file.is_open()) {
        cerr << "There is no file" << device_filename << endl;
        abort();
    }
    device::Device device(device_file, SINGLE_CYCLE, SWAP_CYCLE, CX_CYCLE);

    return device.get_num_qubits();
}

unsigned topo_num(json& conf) {
    // create topology
    cout << "creating topology..." << endl;
    unique_ptr<topo::Topology> topo;
    if (conf["algo"].type() == json::value_t::null) {
        cerr << "Necessary key \"algo\" does not exist." << endl;
        abort();
    } else if (conf["algo"].type() == json::value_t::number_unsigned) {
        unsigned num_qubit = json_get<unsigned>(conf, "algo");
        topo = make_unique<topo::QFTTopology>(num_qubit);
    } else {
        fstream algo_file;
        auto algo_filename = json_get<string>(conf, "algo");
        cout << algo_filename << endl;
        algo_file.open(algo_filename, fstream::in);
        if (!algo_file.is_open()) {
            cerr << "There is no file" << algo_filename << endl;
            abort();
        }
        bool onlyIBM = json_get<unsigned>(conf, "IBM_Gate");
        topo = make_unique<topo::AlgoTopology>(algo_file, onlyIBM);
    }

    return topo->get_num_qubits();
}
