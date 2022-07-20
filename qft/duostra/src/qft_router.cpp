#include "qft_router.hpp"

QFTRouter::QFTRouter(device::Device&& device,
                     std::string& typ,
                     std::string& cost,
                     bool orient) noexcept
    : orient_(orient), apsp_(false), device_(std::move(device)) {
    if (typ == "apsp") {
        apsp_ = true;
        duostra_ = false;
    } else if (typ == "duostra") {
        duostra_ = true;
    } else {
        std::cerr << typ << " is not a router type" << std::endl;
        abort();
    }

    if (cost == "end") {
        greedy_type_ = true;
        apsp_ = true;
    } else if (cost == "start") {
        greedy_type_ = false;
    } else {
        std::cerr << cost << " is not a cost type" << std::endl;
        abort();
    }

    if (apsp_) {
        device_.init_apsp();
    }

    topo_to_dev_.resize(device.get_num_qubits());
    for (unsigned i = 0; i < device.get_num_qubits(); ++i) {
        topo_to_dev_[device.get_qubit(i).get_topo_qubit()] = i;
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
      device_(other.device_),
      topo_to_dev_(std::move(other.topo_to_dev_)) {}

unsigned QFTRouter::get_gate_cost(topo::Gate& gate,
                                  bool min_max,
                                  unsigned apsp_coef) const {
    std::tuple<unsigned, unsigned> device_qubits_idx =
        get_device_qubits_idx(gate);

    if (gate.get_type() == Operator::Single) {
        assert(std::get<1>(device_qubits_idx) == UINT_MAX);
        return device_.get_qubit(std::get<0>(device_qubits_idx))
            .get_avail_time();
    }

    unsigned q0_id = std::get<0>(device_qubits_idx);
    unsigned q1_id = std::get<1>(device_qubits_idx);
    const auto& q0 = device_.get_qubit(q0_id);
    const auto& q1 = device_.get_qubit(q1_id);
    unsigned apsp_cost = 0;
    if (apsp_) {
        apsp_cost = device_.get_shortest_cost(q0_id, q1_id);
        assert(apsp_cost == device_.get_shortest_cost(q1_id, q0_id));
    }
    unsigned avail = min_max
                         ? std::max(q0.get_avail_time(), q1.get_avail_time())
                         : std::min(q0.get_avail_time(), q1.get_avail_time());
    return avail + apsp_cost / apsp_coef;
}

std::vector<device::Operation> QFTRouter::assign_gate(const topo::Gate& gate) {
    std::tuple<unsigned, unsigned> device_qubits_idx =
        get_device_qubits_idx(gate);

    if (gate.get_type() == Operator::Single) {
        assert(std::get<1>(device_qubits_idx) == UINT_MAX);
        device::Operation op = device_.execute_single(
            gate.get_type(), std::get<0>(device_qubits_idx));
        return std::vector<device::Operation>(1, op);
    }
    std::vector<device::Operation> op_list =
        duostra_
            ? device_.duostra_routing(gate.get_type(), device_qubits_idx,
                                      orient_)
            : device_.apsp_routing(gate.get_type(), device_qubits_idx, orient_);
    std::vector<unsigned> change_list = device_.mapping();
    // std::vector<bool> checker(_topo2device.size(), false);

    // i is the idx of device qubit
    for (unsigned i = 0; i < change_list.size(); ++i) {
        unsigned topo_qubit_id = change_list[i];
        if (topo_qubit_id == UINT_MAX) {
            continue;
        }
        // assert(checker[i] == false);
        // checker[i] = true;
        topo_to_dev_[topo_qubit_id] = i;
    }
    // for (unsigned i = 0; i < checker.size(); ++i) {
    //     assert(checker[i]);
    // }
    // std::cout << "Gate: Q" << std::get<0>(gate.get_qubits()) << " Q"
    //           << std::get<1>(gate.get_qubits()) << "\n";
    // device_.print_device_state(std::cout);
    return op_list;
}

bool QFTRouter::is_executable(topo::Gate& gate) const {
    if (gate.get_type() == Operator::Single) {
        assert(std::get<1>(gate.get_qubits()) == UINT_MAX);
        return true;
    }

    std::tuple<unsigned, unsigned> device_qubits_idx =
        get_device_qubits_idx(gate);
    assert(std::get<1>(device_qubits_idx) != UINT_MAX);
    const auto& q0 = device_.get_qubit(std::get<0>(device_qubits_idx));
    const auto& q1 = device_.get_qubit(std::get<1>(device_qubits_idx));
    return q0.is_adj(q1);
}

std::unique_ptr<QFTRouter> QFTRouter::clone() const {
    return std::make_unique<QFTRouter>(*this);
}

std::tuple<unsigned, unsigned> QFTRouter::get_device_qubits_idx(
    const topo::Gate& gate) const {
    unsigned topo_idx_q0 =
        std::get<0>(gate.get_qubits());  // get operation qubit index of
                                         // gate in topology
    unsigned device_idx_q0 =
        topo_to_dev_[topo_idx_q0];  // get device qubit index of the gate

    unsigned device_idx_q1 = UINT_MAX;

    if (gate.get_type() != Operator::Single) {
        // get operation qubit index of
        unsigned topo_idx_q1 = std::get<1>(gate.get_qubits());
        // gate in topology
        assert(topo_idx_q1 != UINT_MAX);
        device_idx_q1 = topo_to_dev_[topo_idx_q1];  // get device qubit
                                                    // index of the gate
    }
    return std::make_tuple(device_idx_q0, device_idx_q1);
}