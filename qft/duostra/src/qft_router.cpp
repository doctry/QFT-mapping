#include "qft_router.hpp"
#include "util.hpp"

using namespace std;

QFTRouter::QFTRouter(device::Device&& device,
                     const string& typ,
                     const string& cost,
                     bool orient) noexcept
    : greedy_type_(false),
      duostra_(false),
      orient_(orient),
      apsp_(false),
      device_(move(device)),
      topo_to_dev_({}) {
    init(typ, cost);
}

void QFTRouter::init(const string& typ, const string& cost) {
    if (typ == "apsp") {
        apsp_ = true;
        duostra_ = false;
    } else if (typ == "duostra") {
        duostra_ = true;
    } else {
        cerr << typ << " is not a router type" << endl;
        abort();
    }

    if (cost == "end") {
        greedy_type_ = true;
        apsp_ = true;
    } else if (cost == "start") {
        greedy_type_ = false;
    } else {
        cerr << cost << " is not a cost type" << endl;
        abort();
    }

    if (apsp_) {
        device_.init_apsp();
    }

    size_t num_qubits = device_.get_num_qubits();
    topo_to_dev_.resize(num_qubits);
    for (size_t i = 0; i < num_qubits; ++i) {
        topo_to_dev_[device_.get_qubit(i).get_topo_qubit()] = i;
    }
}
QFTRouter::QFTRouter(const QFTRouter& other) noexcept
    : greedy_type_(other.greedy_type_),
      duostra_(other.duostra_),
      orient_(other.orient_),
      apsp_(other.apsp_),
      device_(other.device_),
      topo_to_dev_(other.topo_to_dev_) {}

QFTRouter::QFTRouter(QFTRouter&& other) noexcept
    : greedy_type_(other.greedy_type_),
      duostra_(other.duostra_),
      orient_(other.orient_),
      apsp_(other.apsp_),
      device_(move(other.device_)),
      topo_to_dev_(move(other.topo_to_dev_)) {}

size_t QFTRouter::get_gate_cost(const topo::Gate& gate,
                                bool min_max,
                                size_t apsp_coef) const {
    tuple<size_t, size_t> device_qubits_idx = get_device_qubits_idx(gate);

    if (gate.get_type() == Operator::Single) {
        assert(get<1>(device_qubits_idx) == ERROR_CODE);
        return device_.get_qubit(get<0>(device_qubits_idx)).get_avail_time();
    }

    size_t q0_id = get<0>(device_qubits_idx);
    size_t q1_id = get<1>(device_qubits_idx);
    const auto& q0 = device_.get_qubit(q0_id);
    const auto& q1 = device_.get_qubit(q1_id);
    size_t apsp_cost = 0;
    if (apsp_) {
        apsp_cost = device_.get_shortest_cost(q0_id, q1_id);
        assert(apsp_cost == device_.get_shortest_cost(q1_id, q0_id));
    }
    size_t avail = min_max ? max(q0.get_avail_time(), q1.get_avail_time())
                           : min(q0.get_avail_time(), q1.get_avail_time());
    return avail + apsp_cost / apsp_coef;
}

vector<device::Operation> QFTRouter::assign_gate(const topo::Gate& gate) {
    tuple<size_t, size_t> device_qubits_idx = get_device_qubits_idx(gate);

    if (gate.get_type() == Operator::Single) {
        assert(get<1>(device_qubits_idx) == ERROR_CODE);
        device::Operation op =
            device_.execute_single(gate.get_type(), get<0>(device_qubits_idx));
        return vector<device::Operation>(1, op);
    }
    vector<device::Operation> op_list =
        duostra_
            ? device_.duostra_routing(gate.get_type(), device_qubits_idx,
                                      orient_)
            : device_.apsp_routing(gate.get_type(), device_qubits_idx, orient_);
    vector<size_t> change_list = device_.mapping();
    // vector<bool> checker(topo_to_dev_.size(), false);

    // i is the idx of device qubit
    for (size_t i = 0; i < change_list.size(); ++i) {
        size_t topo_qubit_id = change_list[i];
        if (topo_qubit_id == ERROR_CODE) {
            continue;
        }
        // assert(checker[i] == false);
        // checker[i] = true;
        topo_to_dev_[topo_qubit_id] = i;
    }
    // for (size_t i = 0; i < checker.size(); ++i) {
    //     assert(checker[i]);
    // }
    // cout << "Gate: Q" << get<0>(gate.get_qubits()) << " Q"
    //           << get<1>(gate.get_qubits()) << "\n";
    // device_.print_device_state(cout);
    return op_list;
}

bool QFTRouter::is_executable(const topo::Gate& gate) const {
    if (gate.get_type() == Operator::Single) {
        assert(get<1>(gate.get_qubits()) == ERROR_CODE);
        return true;
    }

    tuple<size_t, size_t> device_qubits_idx{get_device_qubits_idx(gate)};
    assert(get<1>(device_qubits_idx) != ERROR_CODE);
    const auto& q0 = device_.get_qubit(get<0>(device_qubits_idx));
    const auto& q1 = device_.get_qubit(get<1>(device_qubits_idx));
    return q0.is_adj(q1);
}

unique_ptr<QFTRouter> QFTRouter::clone() const {
    return make_unique<QFTRouter>(*this);
}

tuple<size_t, size_t> QFTRouter::get_device_qubits_idx(
    const topo::Gate& gate) const {
    size_t topo_idx_q0 =
        get<0>(gate.get_qubits());  // get operation qubit index of
                                    // gate in topology
    size_t device_idx_q0 =
        topo_to_dev_[topo_idx_q0];  // get device qubit index of the gate

    size_t device_idx_q1 = ERROR_CODE;

    if (gate.get_type() != Operator::Single) {
        // get operation qubit index of
        size_t topo_idx_q1 = get<1>(gate.get_qubits());
        // gate in topology
        assert(topo_idx_q1 != ERROR_CODE);
        device_idx_q1 = topo_to_dev_[topo_idx_q1];  // get device qubit
                                                    // index of the gate
    }
    return make_tuple(device_idx_q0, device_idx_q1);
}
