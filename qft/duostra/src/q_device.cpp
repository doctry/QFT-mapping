#include "q_device.hpp"
#include <assert.h>
#include <algorithm>

using namespace device;

std::ostream& device::operator<<(std::ostream& os, device::Operation& op) {
    os << std::left;
    size_t from = std::get<0>(op.duration_);
    size_t to = std::get<1>(op.duration_);
    switch (op.oper_) {
        case Operator::Single:
            os << std::setw(20) << "Operation: Single";
            break;
        case Operator::Swap:
            os << std::setw(20) << "Operation: Swap";
            break;
        case Operator::CX:
            os << std::setw(20) << "Operation: CX";
            break;
        default:
            assert(0);
            break;
    }
    os << "Q" << std::get<0>(op.qubits_) << " Q" << std::get<1>(op.qubits_)
       << "    from: " << std::left << std::setw(10) << from << "to: " << to;
    return os;
}

void device::to_json(json& j, const device::Operation& op) {
    std::string oper;
    switch (op.oper_) {
        case Operator::Single: {
            oper = "Single";
            break;
        }
        case Operator::Swap: {
            oper = "Swap";
            break;
        }
        case Operator::CX: {
            oper = "CX";
            break;
        }
        default: { assert(false); }
    }

    j["Operator"] = oper;
    j["Qubits"] = {std::get<0>(op.qubits_), std::get<1>(op.qubits_)};
    j["duration"] = {std::get<0>(op.duration_), std::get<1>(op.duration_)};
}

Qubit::Qubit(const size_t i)
    : id_(i),
      topo_qubit_(size_t(-1)),
      occupied_until_(0),
      marked_(false),
      cost_(0),
      taken_(false) {}

Qubit::Qubit(const Qubit& other)
    : id_(other.id_),
      adj_list_(other.adj_list_),
      topo_qubit_(other.topo_qubit_),
      occupied_until_(other.occupied_until_),
      marked_(other.marked_),
      cost_(other.cost_),
      taken_(other.taken_) {}

Qubit::Qubit(Qubit&& other)
    : id_(other.id_),
      adj_list_(std::move(other.adj_list_)),
      topo_qubit_(other.topo_qubit_),
      occupied_until_(other.occupied_until_),
      marked_(other.marked_),
      cost_(other.cost_),
      taken_(other.taken_) {}

void Qubit::add_adj(size_t i) {
    adj_list_.push_back(i);
}

size_t Qubit::get_id() const {
    return id_;
}

size_t Qubit::get_avail_time() const {
    return occupied_until_;
}

bool Qubit::is_adj(const Qubit& other) const {
    return std::find(adj_list_.begin(), adj_list_.end(), other.id_) !=
           adj_list_.end();
}

size_t Qubit::get_topo_qubit() const {
    return topo_qubit_;
}

void Qubit::set_topo_qubit(size_t i) {
    topo_qubit_ = i;
}

void Qubit::set_occupied_time(size_t t) {
    if (occupied_until_ < t) {
        occupied_until_ = t;
    }
}

const std::vector<size_t>& Qubit::get_adj_list() const {
    return adj_list_;
}

size_t Qubit::get_cost() const {
    return cost_;
}

bool Qubit::is_marked() const {
    return marked_;
}

bool Qubit::is_taken() const {
    return taken_;
}
bool Qubit::get_swtch() const {
    return swtch_;
}

size_t Qubit::get_pred() const {
    return pred_;
}

size_t Qubit::get_swap_time() const {
    return swap_time_;
}

void Qubit::mark(bool swtch, size_t pred) {
    // assert(marked_ == false);
    marked_ = true;
    swtch_ = swtch;
    pred_ = pred;
}

void Qubit::reset() {
    marked_ = false;
    taken_ = false;
    cost_ = occupied_until_;
}

void Qubit::take_route(size_t cost, size_t swap_time) {
    cost_ = cost;
    swap_time_ = swap_time;
    taken_ = true;
}

bool device::op_order(const device::Operation& a, const device::Operation& b) {
    return a.get_op_time() < b.get_op_time();
}

Device::Device(std::fstream& file, size_t u, size_t s, size_t cx) noexcept
    : SINGLE_CYCLE(u), SWAP_CYCLE(s), CX_CYCLE(cx) {
    size_t num;
    file >> num;

    for (size_t i = 0; i < num; ++i) {
        size_t id;
        file >> id;
        assert(i == id);
        size_t num_adj;
        file >> num_adj;

        Qubit qubit(i);
        for (size_t j = 0; j < num_adj; ++j) {
            file >> id;
            qubit.add_adj(id);
        }

        qubits_.push_back(std::move(qubit));
    }
}

Device::Device(std::vector<std::vector<size_t>>& adj_lists,
               size_t u,
               size_t s,
               size_t cx) noexcept
    : SINGLE_CYCLE(u), SWAP_CYCLE(s), CX_CYCLE(cx) {
    for (size_t i = 0; i < adj_lists.size(); ++i) {
        std::vector<size_t>& adj_list = adj_lists[i];

        Qubit qubit{i};
        for (size_t j = 0; j < adj_list.size(); ++j) {
            qubit.add_adj(adj_list[j]);
        }

        qubits_.push_back(std::move(qubit));
    }
}

Device::Device(const Device& other) noexcept
    : SINGLE_CYCLE(other.SINGLE_CYCLE),
      SWAP_CYCLE(other.SWAP_CYCLE),
      CX_CYCLE(other.CX_CYCLE),
      qubits_(other.qubits_),
      shortest_path_(other.shortest_path_),
      shortest_cost_(other.shortest_cost_) {}

Device::Device(Device&& other) noexcept
    : SINGLE_CYCLE(other.SINGLE_CYCLE),
      SWAP_CYCLE(other.SWAP_CYCLE),
      CX_CYCLE(other.CX_CYCLE),
      qubits_(std::move(other.qubits_)),
      shortest_path_(std::move(other.shortest_path_)),
      shortest_cost_(std::move(other.shortest_cost_)) {}

size_t Device::get_num_qubits() const {
    return qubits_.size();
}

void Device::init_apsp() {
    std::cout << "calculating apsp..." << std::endl;
    torch::Tensor adj_mat =
        torch::zeros({int(qubits_.size()), int(qubits_.size())});
    for (size_t i = 0; i < qubits_.size(); ++i) {
        const std::vector<size_t>& adj_list = qubits_[i].get_adj_list();
        for (size_t j = 0; j < adj_list.size(); ++j) {
            adj_mat.index_put_({int(i), int(adj_list[j])}, 1);
        }
    }

    ShortestPath shortest_path_torch = apsp(adj_mat);

    shortest_cost_.reserve(qubits_.size());
    for (size_t i = 0; i < qubits_.size(); ++i) {
        std::vector<size_t> arr(qubits_.size(), 0);
        for (size_t j = 0; j < qubits_.size(); ++j) {
            arr[j] = size_t(
                shortest_path_torch.cost.index({int(i), int(j)}).item<int>());
        }
        shortest_cost_.push_back(std::move(arr));
    }

    shortest_path_.reserve(qubits_.size());
    for (size_t i = 0; i < qubits_.size(); ++i) {
        std::vector<size_t> arr(qubits_.size(), 0);
        for (size_t j = 0; j < qubits_.size(); ++j) {
            arr[j] = size_t(shortest_path_torch.pointer.index({int(i), int(j)})
                                .item<int>());
        }
        shortest_path_.push_back(std::move(arr));
    }
}

Qubit& Device::get_qubit(size_t i) {
    return qubits_[i];
}

const Qubit& Device::get_qubit(size_t i) const {
    return qubits_[i];
}

void reset_qubit(Qubit& q) {
    q.reset();
}

void Device::reset() {
    for (size_t i = 0; i < qubits_.size(); ++i) {
        qubits_[i].reset();
    }
}

device::Operation Device::execute_single(Operator oper, size_t q) {
    assert(oper == Operator::Single);
    Qubit& qubit = qubits_[q];
    size_t starttime = qubit.get_avail_time();
    size_t endtime = starttime + SINGLE_CYCLE;
    qubit.set_occupied_time(endtime);
    qubit.reset();
    Operation op(oper, std::make_tuple(q, size_t(-1)),
                 std::make_tuple(starttime, endtime));
#ifdef DEBUG
    std::cout << op << "\n";
#endif
    return op;
}

std::vector<device::Operation> Device::duostra_routing(
    Operator op,
    std::tuple<size_t, size_t> qs,
    bool orient) {
    assert(op == Operator::CX || op == Operator::Single);
    size_t q0_idx = std::get<0>(qs);  // source 0
    size_t q1_idx = std::get<1>(qs);  // source 1

    if (get_qubit(q0_idx).get_avail_time() >
        get_qubit(q1_idx).get_avail_time()) {
        std::swap(q0_idx, q1_idx);
    } else if (get_qubit(q0_idx).get_avail_time() ==
               get_qubit(q1_idx).get_avail_time()) {
        if (orient && get_qubit(q0_idx).get_topo_qubit() >
                          get_qubit(q1_idx).get_topo_qubit()) {
            std::swap(q0_idx, q1_idx);
        }
    }

    Qubit& t0 = get_qubit(q0_idx);  // target 0
    Qubit& t1 = get_qubit(q1_idx);  // target 1

    // priority queue
    std::priority_queue<device::AStarNode, std::vector<device::AStarNode>,
                        device::AStarComp>
        pq;

    t0.mark(false, t0.get_id());
    t0.take_route(t0.get_cost(), 0);
    t1.mark(true, t1.get_id());
    t1.take_route(t1.get_cost(), 0);
    std::tuple<bool, size_t> touch0 = touch_adj(t0, pq, false);
    bool is_adj = std::get<0>(touch0);
#ifdef DEBUG
    std::tuple<bool, size_t> touch1 = touch_adj(t1, pq, true);
    assert(is_adj == std::get<0>(touch1));
#else
    touch_adj(t1, pq, true);
#endif

    while (!is_adj)  // set not adjacent
    {
        device::AStarNode next(pq.top());
        pq.pop();
        size_t q_next_idx = next.get_id();
        Qubit& q_next = get_qubit(q_next_idx);
        assert(q_next.get_swtch() == next.get_swtch());

        size_t cost = next.get_cost();
        assert(cost >= SWAP_CYCLE);
        size_t op_time = cost - SWAP_CYCLE;
        q_next.take_route(cost, op_time);
        std::tuple<bool, size_t> touch =
            touch_adj(q_next, pq, next.get_swtch());
        is_adj = std::get<0>(touch);
        if (is_adj) {
            if (next.get_swtch())  // 0 get true means touch 1's set
            {
                q0_idx = std::get<1>(touch);
                q1_idx = q_next_idx;
            } else {
                q0_idx = q_next_idx;
                q1_idx = std::get<1>(touch);
            }
        }
    }
    std::vector<Operation> ops =
        traceback(op, get_qubit(q0_idx), get_qubit(q1_idx), t0, t1);

#ifdef DEBUG
    for (size_t i = 0; i < ops.size(); ++i) {
        std::cout << ops[i] << "\n";
    }
#endif

#ifdef DEBUG
    std::vector<bool> checker(qubits_.size(), false);
#endif
    for (size_t i = 0; i < qubits_.size(); ++i) {
        Qubit& qubit = qubits_[i];
        qubit.reset();
        assert(qubit.get_topo_qubit() < qubits_.size());
#ifdef DEBUG
        if (i != size_t(-1)) {
            assert(checker[i] == false);
            checker[i] = true;
        }
#endif
    }
    return ops;
}

std::tuple<bool, size_t> Device::touch_adj(
    Qubit& qubit,
    std::priority_queue<device::AStarNode,
                        std::vector<device::AStarNode>,
                        device::AStarComp>& pq,
    bool swtch) {
    const std::vector<size_t>& adj_list = qubit.get_adj_list();
    for (size_t i = 0; i < adj_list.size(); ++i) {
        Qubit& adj = qubits_[adj_list[i]];
        if (adj.is_marked())  // already in the queue
        {
            if (adj.is_taken()) {
                if (adj.get_swtch() != swtch)  // touch target
                {
                    assert(adj.get_id() == adj_list[i]);
                    return std::make_tuple(true, adj.get_id());
                }
            }
            continue;
        }

        size_t cost =
            std::max(qubit.get_cost(), adj.get_avail_time()) + SWAP_CYCLE;
        size_t estimated_cost = 0;
        size_t heuristic_cost = cost + estimated_cost;
        adj.mark(swtch, qubit.get_id());

        pq.push(device::AStarNode(heuristic_cost, adj.get_id(), swtch));
    }
    return std::make_tuple(false, size_t(-1));
}

std::vector<device::Operation> Device::traceback(Operator op,
                                                 Qubit& q0,
                                                 Qubit& q1,
                                                 Qubit& t0,
                                                 Qubit& t1) {
    assert(t0.get_id() == t0.get_pred());
    assert(t1.get_id() == t1.get_pred());

    assert(q0.is_adj(q1));
    std::vector<Operation> ops;

    size_t op_time = std::max(q0.get_cost(), q1.get_cost());

    assert(op == Operator::CX);
    Operation CX_gate(Operator::CX, std::make_tuple(q0.get_id(), q1.get_id()),
                      std::make_tuple(op_time, op_time + CX_CYCLE));
    ops.push_back(CX_gate);

    size_t trace_0 = q0.get_id();
    size_t trace_1 = q1.get_id();
    while (trace_0 != t0.get_id())  // trace 0
    {
        Qubit& q_trace_0 = get_qubit(trace_0);
        size_t trace_pred_0 = q_trace_0.get_pred();

        size_t swap_time = q_trace_0.get_swap_time();
        Operation swap_gate(Operator::Swap,
                            std::make_tuple(trace_0, trace_pred_0),
                            std::make_tuple(swap_time, swap_time + SWAP_CYCLE));
        ops.push_back(swap_gate);

        trace_0 = trace_pred_0;
    }
    while (trace_1 != t1.get_id())  // trace 1
    {
        Qubit& q_trace_1 = get_qubit(trace_1);
        size_t trace_pred_1 = q_trace_1.get_pred();

        size_t swap_time = q_trace_1.get_swap_time();
        Operation swap_gate(Operator::Swap,
                            std::make_tuple(trace_1, trace_pred_1),
                            std::make_tuple(swap_time, swap_time + SWAP_CYCLE));
        ops.push_back(swap_gate);

        trace_1 = trace_pred_1;
    }

    std::sort(ops.begin(), ops.end(), op_order);
    for (size_t i = 0; i < ops.size(); ++i) {
        apply_gate(ops[i]);
    }

    return ops;
}

std::vector<device::Operation>
Device::apsp_routing(Operator op, std::tuple<size_t, size_t> qs, bool orient) {
    std::vector<device::Operation> ops;
    size_t s0_idx = std::get<0>(qs);
    size_t s1_idx = std::get<1>(qs);
    size_t q0_idx = s0_idx;
    size_t q1_idx = s1_idx;

    while (!get_qubit(q0_idx).is_adj(get_qubit(q1_idx))) {
        std::tuple<size_t, size_t> q0_next_cost =
            next_swap_cost(q0_idx, s1_idx);
        std::tuple<size_t, size_t> q1_next_cost =
            next_swap_cost(q1_idx, s0_idx);

        size_t q0_next = std::get<0>(q0_next_cost);
        size_t q0_cost = std::get<1>(q0_next_cost);
        size_t q1_next = std::get<0>(q1_next_cost);
        size_t q1_cost = std::get<1>(q1_next_cost);

        if ((q0_cost < q1_cost) || ((q0_cost == q1_cost) && orient &&
                                    get_qubit(q0_idx).get_topo_qubit() <
                                        get_qubit(q1_idx).get_topo_qubit())) {
            device::Operation oper(
                Operator::Swap, std::make_tuple(q0_idx, q0_next),
                std::make_tuple(q0_cost, q0_cost + SWAP_CYCLE));
            apply_gate(oper);
            ops.push_back(std::move(oper));
            q0_idx = q0_next;
        } else {
            device::Operation oper(
                Operator::Swap, std::make_tuple(q1_idx, q1_next),
                std::make_tuple(q0_cost, q0_cost + SWAP_CYCLE));
            apply_gate(oper);
            ops.push_back(std::move(oper));
            q1_idx = q1_next;
        }
    }
    assert(get_qubit(q1_idx).is_adj(get_qubit(q0_idx)));

    size_t gate_cost = std::max(get_qubit(q0_idx).get_avail_time(),
                                get_qubit(q1_idx).get_avail_time());
    assert(op == Operator::CX);
    Operation CX_gate(Operator::CX, std::make_tuple(q0_idx, q1_idx),
                      std::make_tuple(gate_cost, gate_cost + CX_CYCLE));
    apply_gate(CX_gate);
    ops.push_back(CX_gate);

    return ops;
}

std::tuple<size_t, size_t> Device::next_swap_cost(size_t source,
                                                  size_t target) {
    size_t next_idx = shortest_path_[source][target];
    Qubit& q_source = get_qubit(source);
    Qubit& q_next = get_qubit(next_idx);
    size_t cost = std::max(q_source.get_avail_time(), q_next.get_avail_time());

    assert(q_source.is_adj(q_next));

    return std::make_tuple(next_idx, cost);
}

void Device::place(std::vector<size_t>& assign) {
    for (size_t i = 0; i < assign.size(); ++i) {
        Qubit& q = qubits_[assign[i]];
        assert(q.get_topo_qubit() == size_t(-1));
        q.set_topo_qubit(i);
    }
}

std::vector<size_t> Device::mapping() const {
    std::vector<size_t> ret;
    ret.reserve(qubits_.size());

    for (const Qubit& qubit : qubits_) {
        ret.push_back(qubit.get_topo_qubit());
    }
    return ret;
}

size_t Device::get_shortest_cost(size_t i, size_t j) const {
    return shortest_cost_[i][j];
}

void Device::apply_gate(const Operation& op) {
    Operator gate = op.get_operator();
    std::tuple<size_t, size_t> qubits = op.get_qubits();
    Qubit& q0 = get_qubit(std::get<0>(qubits));
    Qubit& q1 = get_qubit(std::get<1>(qubits));
    size_t t = op.get_op_time();

    switch (gate) {
        case Operator::Swap: {  // swap topo qubit
            assert(t + SWAP_CYCLE == op.get_cost());

            size_t temp = q0.get_topo_qubit();
            q0.set_topo_qubit(q1.get_topo_qubit());
            q1.set_topo_qubit(temp);
            q0.set_occupied_time(t + SWAP_CYCLE);
            q1.set_occupied_time(t + SWAP_CYCLE);
            break;
        }

        case Operator::CX: {
            assert(t + CX_CYCLE == op.get_cost());

            q0.set_occupied_time(t + CX_CYCLE);
            q1.set_occupied_time(t + CX_CYCLE);
            break;
        }

        default: { assert(false); }
    }
}

std::ostream& device::operator<<(std::ostream& os, const Qubit& q) {
    return os << "Q" << q.get_id() << ": topo qubit: " << q.get_topo_qubit()
              << " occupied until: " << q.get_avail_time();
}

void Device::print_device_state(std::ostream& out) {
    for (size_t i = 0; i < qubits_.size(); ++i) {
        out << qubits_[i] << "\n";
    }
    out << "\n";
}
