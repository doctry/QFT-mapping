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

    topo_to_dev_.resize(device_.get_num_qubits());
    for (size_t i = 0; i < device_.get_num_qubits(); ++i) {
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
      device_(std::move(other.device_)),
      topo_to_dev_(std::move(other.topo_to_dev_)) {}

size_t QFTRouter::get_gate_cost(topo::Gate& gate,
                                bool min_max,
                                size_t apsp_coef) const {
    std::tuple<size_t, size_t> device_qubits_idx = get_device_qubits_idx(gate);

    if (gate.get_type() == Operator::Single) {
        assert(std::get<1>(device_qubits_idx) == size_t(-1));
        return device_.get_qubit(std::get<0>(device_qubits_idx))
            .get_avail_time();
    }

    size_t q0_id = std::get<0>(device_qubits_idx);
    size_t q1_id = std::get<1>(device_qubits_idx);
    const auto& q0 = device_.get_qubit(q0_id);
    const auto& q1 = device_.get_qubit(q1_id);
    size_t apsp_cost = 0;
    if (apsp_) {
        apsp_cost = device_.get_shortest_cost(q0_id, q1_id);
        assert(apsp_cost == device_.get_shortest_cost(q1_id, q0_id));
    }
    size_t avail = min_max ? std::max(q0.get_avail_time(), q1.get_avail_time())
                           : std::min(q0.get_avail_time(), q1.get_avail_time());
    return avail + apsp_cost / apsp_coef;
}

std::vector<device::Operation> QFTRouter::assign_gate(const topo::Gate& gate) {
    std::tuple<size_t, size_t> device_qubits_idx = get_device_qubits_idx(gate);

    if (gate.get_type() == Operator::Single) {
        assert(std::get<1>(device_qubits_idx) == size_t(-1));
        device::Operation op = device_.execute_single(
            gate.get_type(), std::get<0>(device_qubits_idx));
        return std::vector<device::Operation>(1, op);
    }
    std::vector<device::Operation> op_list =
        duostra_
            ? device_.duostra_routing(gate.get_type(), device_qubits_idx,
                                      orient_)
            : device_.apsp_routing(gate.get_type(), device_qubits_idx, orient_);
    std::vector<size_t> change_list = device_.mapping();
    // std::vector<bool> checker(_topo2device.size(), false);

    // i is the idx of device qubit
    for (size_t i = 0; i < change_list.size(); ++i) {
        size_t topo_qubit_id = change_list[i];
        if (topo_qubit_id == size_t(-1)) {
            continue;
        }
        // assert(checker[i] == false);
        // checker[i] = true;
        topo_to_dev_[topo_qubit_id] = i;
    }
    // for (size_t i = 0; i < checker.size(); ++i) {
    //     assert(checker[i]);
    // }
    // std::cout << "Gate: Q" << std::get<0>(gate.get_qubits()) << " Q"
    //           << std::get<1>(gate.get_qubits()) << "\n";
    // device_.print_device_state(std::cout);
    return op_list;
}

bool QFTRouter::is_executable(topo::Gate& gate) const {
    if (gate.get_type() == Operator::Single) {
        assert(std::get<1>(gate.get_qubits()) == size_t(-1));
        return true;
    }

    std::tuple<size_t, size_t> device_qubits_idx = get_device_qubits_idx(gate);
    assert(std::get<1>(device_qubits_idx) != size_t(-1));
    const auto& q0 = device_.get_qubit(std::get<0>(device_qubits_idx));
    const auto& q1 = device_.get_qubit(std::get<1>(device_qubits_idx));
    return q0.is_adj(q1);
}

std::unique_ptr<QFTRouter> QFTRouter::clone() const {
    return std::make_unique<QFTRouter>(*this);
}

std::tuple<size_t, size_t> QFTRouter::get_device_qubits_idx(
    const topo::Gate& gate) const {
    size_t topo_idx_q0 =
        std::get<0>(gate.get_qubits());  // get operation qubit index of
                                         // gate in topology
    size_t device_idx_q0 =
        topo_to_dev_[topo_idx_q0];  // get device qubit index of the gate

    size_t device_idx_q1 = size_t(-1);

    if (gate.get_type() != Operator::Single) {
        // get operation qubit index of
        size_t topo_idx_q1 = std::get<1>(gate.get_qubits());
        // gate in topology
        assert(topo_idx_q1 != size_t(-1));
        device_idx_q1 = topo_to_dev_[topo_idx_q1];  // get device qubit
                                                    // index of the gate
    }
    return std::make_tuple(device_idx_q0, device_idx_q1);
}
