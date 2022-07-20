#include "q_device.hpp"
#include <assert.h>
#include <algorithm>

std::ostream& device::operator<<(std::ostream& os, device::Operation& op) {
    os << std::left;
    unsigned from = std::get<0>(op.duration_);
    unsigned to = std::get<1>(op.duration_);
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
        default: {
            assert(false);
        }
    }

    j["Operator"] = oper;
    j["Qubits"] = {std::get<0>(op.qubits_), std::get<1>(op.qubits_)};
    j["duration"] = {std::get<0>(op.duration_), std::get<1>(op.duration_)};
}

device::Qubit::Qubit(const unsigned i)
    : id_(i),
      topo_qubit_(UINT_MAX),
      occupied_until_(0),
      marked_(false),
      cost_(0),
      taken_(false) {}

device::Qubit::Qubit(const Qubit& other)
    : id_(other.id_),
      adj_list_(other.adj_list_),
      topo_qubit_(other.topo_qubit_),
      occupied_until_(other.occupied_until_),
      marked_(other.marked_),
      cost_(other.cost_),
      taken_(other.taken_) {}

device::Qubit::Qubit(Qubit&& other)
    : id_(other.id_),
      adj_list_(std::move(other.adj_list_)),
      topo_qubit_(other.topo_qubit_),
      occupied_until_(other.occupied_until_),
      marked_(other.marked_),
      cost_(other.cost_),
      taken_(other.taken_) {}

void device::Qubit::add_adj(unsigned i) {
    adj_list_.push_back(i);
}

unsigned device::Qubit::get_id() const {
    return id_;
}

unsigned device::Qubit::get_avail_time() const {
    return occupied_until_;
}

bool device::Qubit::is_adj(const Qubit& other) const {
    return std::find(adj_list_.begin(), adj_list_.end(), other.id_) !=
           adj_list_.end();
}

unsigned device::Qubit::get_topo_qubit() const {
    return topo_qubit_;
}

void device::Qubit::set_topo_qubit( unsigned i) {
    topo_qubit_ = i;
}

void device::Qubit::set_occupied_time( unsigned t) {
    if (occupied_until_ < t) {
        occupied_until_ = t;
    }
}

const std::vector<unsigned>& device::Qubit::get_adj_list() const {
    return adj_list_;
}

unsigned device::Qubit::get_cost() const {
    return cost_;
}

bool device::Qubit::is_marked() const {
    return marked_;
}

bool device::Qubit::is_taken() const {
    return taken_;
}
bool device::Qubit::get_swtch() const {
    return swtch_;
}

unsigned device::Qubit::get_pred() const {
    return pred_;
}

unsigned device::Qubit::get_swap_time() const {
    return swap_time_;
}

void device::Qubit::mark(bool swtch, unsigned pred) {
    // assert(marked_ == false);
    marked_ = true;
    swtch_ = swtch;
    pred_ = pred;
}

void device::Qubit::reset() {
    marked_ = false;
    taken_ = false;
    cost_ = occupied_until_;
}

void device::Qubit::take_route(unsigned cost, unsigned swap_time) {
    cost_ = cost;
    swap_time_ = swap_time;
    taken_ = true;
}

bool device::op_order(const device::Operation& a, const device::Operation& b) {
    return a.get_op_time() < b.get_op_time();
}

device::Device::Device(std::fstream& file,
                       unsigned u,
                       unsigned s,
                       unsigned cx) noexcept
    : SINGLE_CYCLE(u), SWAP_CYCLE(s), CX_CYCLE(cx) {
    unsigned num;
    file >> num;

    for (unsigned i = 0; i < num; ++i) {
        unsigned id;
        file >> id;
        assert(i == id);
        unsigned num_adj;
        file >> num_adj;

        device::Qubit qubit(i);
        for (unsigned j = 0; j < num_adj; ++j) {
            file >> id;
            qubit.add_adj(id);
        }

        qubits_.push_back(std::move(qubit));
    }
}

device::Device::Device(std::vector<std::vector<unsigned>>& adj_lists,
                       unsigned u,
                       unsigned s,
                       unsigned cx) noexcept
    : SINGLE_CYCLE(u), SWAP_CYCLE(s), CX_CYCLE(cx) {
    for (unsigned i = 0; i < adj_lists.size(); ++i) {
        std::vector<unsigned>& adj_list = adj_lists[i];

        device::Qubit qubit(i);
        for (unsigned j = 0; j < adj_list.size(); ++j) {
            qubit.add_adj(adj_list[j]);
        }

        qubits_.push_back(std::move(qubit));
    }
}

device::Device::Device(const Device& other) noexcept
    : SINGLE_CYCLE(other.SINGLE_CYCLE),
      SWAP_CYCLE(other.SWAP_CYCLE),
      CX_CYCLE(other.CX_CYCLE),
      qubits_(other.qubits_),
      shortest_path_(other.shortest_path_),
      shortest_cost_(other.shortest_cost_) {}

device::Device::Device(Device&& other) noexcept
    : SINGLE_CYCLE(other.SINGLE_CYCLE),
      SWAP_CYCLE(other.SWAP_CYCLE),
      CX_CYCLE(other.CX_CYCLE),
      qubits_(std::move(other.qubits_)),
      shortest_path_(std::move(other.shortest_path_)),
      shortest_cost_(std::move(other.shortest_cost_)) {}

unsigned device::Device::get_num_qubits() const {
    return qubits_.size();
}

void device::Device::init_apsp() {
    std::cout << "calculating apsp..." << std::endl;
    torch::Tensor adj_mat =
        torch::zeros({int(qubits_.size()), int(qubits_.size())});
    for (unsigned i = 0; i < qubits_.size(); ++i) {
        const std::vector<unsigned>& adj_list = qubits_[i].get_adj_list();
        for (unsigned j = 0; j < adj_list.size(); ++j) {
            adj_mat.index_put_({int(i), int(adj_list[j])}, 1);
        }
    }

    ShortestPath shortest_path_torch = apsp(adj_mat);

    shortest_cost_.reserve(qubits_.size());
    for (unsigned i = 0; i < qubits_.size(); ++i) {
        std::vector<unsigned> arr(qubits_.size(), 0);
        for (unsigned j = 0; j < qubits_.size(); ++j) {
            arr[j] = unsigned(
                shortest_path_torch.cost.index({int(i), int(j)}).item<int>());
        }
        shortest_cost_.push_back(std::move(arr));
    }

    shortest_path_.reserve(qubits_.size());
    for (unsigned i = 0; i < qubits_.size(); ++i) {
        std::vector<unsigned> arr(qubits_.size(), 0);
        for (unsigned j = 0; j < qubits_.size(); ++j) {
            arr[j] =
                unsigned(shortest_path_torch.pointer.index({int(i), int(j)})
                             .item<int>());
        }
        shortest_path_.push_back(std::move(arr));
    }
}

device::Qubit& device::Device::get_qubit(unsigned i) {
    return qubits_[i];
}

const device::Qubit& device::Device::get_qubit(unsigned i) const {
    return qubits_[i];
}

void reset_qubit(device::Qubit& q) {
    q.reset();
}

void device::Device::reset() {
    for (unsigned i = 0; i < qubits_.size(); ++i) {
        qubits_[i].reset();
    }
}

device::Operation device::Device::execute_single(Operator oper, unsigned q) {
    assert(oper == Operator::Single);
    Qubit& qubit = qubits_[q];
    unsigned starttime = qubit.get_avail_time();
    unsigned endtime = starttime + SINGLE_CYCLE;
    qubit.set_occupied_time(endtime);
    qubit.reset();
    Operation op(oper, std::make_tuple(q, UINT_MAX),
                 std::make_tuple(starttime, endtime));
#ifdef DEBUG
    std::cout << op << "\n";
#endif
    return op;
}

std::vector<device::Operation> device::Device::duostra_routing(
    Operator op,
    std::tuple<unsigned, unsigned> qs,
    bool orient) {
    assert(op == Operator::CX || op == Operator::Single);
    unsigned q0_idx = std::get<0>(qs);  // source 0
    unsigned q1_idx = std::get<1>(qs);  // source 1

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

    device::Qubit& t0 = get_qubit(q0_idx);  // target 0
    device::Qubit& t1 = get_qubit(q1_idx);  // target 1

    // priority queue
    std::priority_queue<device::AStarNode, std::vector<device::AStarNode>,
                        device::AStarComp>
        pq;

    t0.mark(false, t0.get_id());
    t0.take_route(t0.get_cost(), 0);
    t1.mark(true, t1.get_id());
    t1.take_route(t1.get_cost(), 0);
    std::tuple<bool, unsigned> touch0 = touch_adj(t0, pq, false);
    bool is_adj = std::get<0>(touch0);
#ifdef DEBUG
    std::tuple<bool, unsigned> touch1 = touch_adj(t1, pq, true);
    assert(is_adj == std::get<0>(touch1));
#else
    touch_adj(t1, pq, true);
#endif

    while (!is_adj)  // set not adjacent
    {
        device::AStarNode next(pq.top());
        pq.pop();
        unsigned q_next_idx = next.get_id();
        device::Qubit& q_next = get_qubit(q_next_idx);
        assert(q_next.get_swtch() == next.get_swtch());

        unsigned cost = next.get_cost();
        assert(cost >= SWAP_CYCLE);
        unsigned op_time = cost - SWAP_CYCLE;
        q_next.take_route(cost, op_time);
        std::tuple<bool, unsigned> touch =
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
    for (unsigned i = 0; i < ops.size(); ++i) {
        std::cout << ops[i] << "\n";
    }
#endif

#ifdef DEBUG
    std::vector<bool> checker(qubits_.size(), false);
#endif
    for (unsigned i = 0; i < qubits_.size(); ++i) {
        device::Qubit& qubit = qubits_[i];
        qubit.reset();
        assert(qubit.get_topo_qubit() < qubits_.size());
#ifdef DEBUG
        if (i != UINT_MAX) {
            assert(checker[i] == false);
            checker[i] = true;
        }
#endif
    }
    return ops;
}

std::tuple<bool, unsigned> device::Device::touch_adj(
    device::Qubit& qubit,
    std::priority_queue<device::AStarNode,
                        std::vector<device::AStarNode>,
                        device::AStarComp>& pq,
    bool swtch) {
    const std::vector<unsigned>& adj_list = qubit.get_adj_list();
    for (unsigned i = 0; i < adj_list.size(); ++i) {
        device::Qubit& adj = qubits_[adj_list[i]];
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

        unsigned cost =
            std::max(qubit.get_cost(), adj.get_avail_time()) + SWAP_CYCLE;
        unsigned estimated_cost = 0;
        unsigned heuristic_cost = cost + estimated_cost;
        adj.mark(swtch, qubit.get_id());

        pq.push(device::AStarNode(heuristic_cost, adj.get_id(), swtch));
    }
    return std::make_tuple(false, UINT_MAX);
}

std::vector<device::Operation> device::Device::traceback(Operator op,
                                                         device::Qubit& q0,
                                                         device::Qubit& q1,
                                                         device::Qubit& t0,
                                                         device::Qubit& t1) {
    assert(t0.get_id() == t0.get_pred());
    assert(t1.get_id() == t1.get_pred());

    assert(q0.is_adj(q1));
    std::vector<Operation> ops;

    unsigned op_time = std::max(q0.get_cost(), q1.get_cost());

    assert(op == Operator::CX);
    Operation CX_gate(Operator::CX, std::make_tuple(q0.get_id(), q1.get_id()),
                      std::make_tuple(op_time, op_time + CX_CYCLE));
    ops.push_back(CX_gate);

    unsigned trace_0 = q0.get_id();
    unsigned trace_1 = q1.get_id();
    while (trace_0 != t0.get_id())  // trace 0
    {
        device::Qubit& q_trace_0 = get_qubit(trace_0);
        unsigned trace_pred_0 = q_trace_0.get_pred();

        unsigned swap_time = q_trace_0.get_swap_time();
        Operation swap_gate(Operator::Swap,
                            std::make_tuple(trace_0, trace_pred_0),
                            std::make_tuple(swap_time, swap_time + SWAP_CYCLE));
        ops.push_back(swap_gate);

        trace_0 = trace_pred_0;
    }
    while (trace_1 != t1.get_id())  // trace 1
    {
        device::Qubit& q_trace_1 = get_qubit(trace_1);
        unsigned trace_pred_1 = q_trace_1.get_pred();

        unsigned swap_time = q_trace_1.get_swap_time();
        Operation swap_gate(Operator::Swap,
                            std::make_tuple(trace_1, trace_pred_1),
                            std::make_tuple(swap_time, swap_time + SWAP_CYCLE));
        ops.push_back(swap_gate);

        trace_1 = trace_pred_1;
    }

    std::sort(ops.begin(), ops.end(), op_order);
    for (unsigned i = 0; i < ops.size(); ++i) {
        apply_gate(ops[i]);
    }

    return ops;
}

std::vector<device::Operation> device::Device::apsp_routing(
    Operator op,
    std::tuple<unsigned, unsigned> qs,
    bool orient) {
    std::vector<device::Operation> ops;
    unsigned s0_idx = std::get<0>(qs);
    unsigned s1_idx = std::get<1>(qs);
    unsigned q0_idx = s0_idx;
    unsigned q1_idx = s1_idx;

    while (!get_qubit(q0_idx).is_adj(get_qubit(q1_idx))) {
        std::tuple<unsigned, unsigned> q0_next_cost =
            next_swap_cost(q0_idx, s1_idx);
        std::tuple<unsigned, unsigned> q1_next_cost =
            next_swap_cost(q1_idx, s0_idx);

        unsigned q0_next = std::get<0>(q0_next_cost);
        unsigned q0_cost = std::get<1>(q0_next_cost);
        unsigned q1_next = std::get<0>(q1_next_cost);
        unsigned q1_cost = std::get<1>(q1_next_cost);

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

    unsigned gate_cost = std::max(get_qubit(q0_idx).get_avail_time(),
                                  get_qubit(q1_idx).get_avail_time());
    assert(op == Operator::CX);
    Operation CX_gate(Operator::CX, std::make_tuple(q0_idx, q1_idx),
                      std::make_tuple(gate_cost, gate_cost + CX_CYCLE));
    apply_gate(CX_gate);
    ops.push_back(CX_gate);

    return ops;
}

std::tuple<unsigned, unsigned> device::Device::next_swap_cost(unsigned source,
                                                              unsigned target) {
    unsigned next_idx = shortest_path_[source][target];
    Qubit& q_source = get_qubit(source);
    Qubit& q_next = get_qubit(next_idx);
    unsigned cost =
        std::max(q_source.get_avail_time(), q_next.get_avail_time());

    assert(q_source.is_adj(q_next));

    return std::make_tuple(next_idx, cost);
}

void device::Device::place(std::vector<unsigned>& assign) {
    for (unsigned i = 0; i < assign.size(); ++i) {
        device::Qubit& q = qubits_[assign[i]];
        assert(q.get_topo_qubit() == UINT_MAX);
        q.set_topo_qubit(i);
    }
}

std::vector<unsigned> device::Device::mapping() const {
    std::vector<unsigned> ret;
    ret.reserve(qubits_.size());

    for (const device::Qubit& qubit : qubits_) {
        ret.push_back(qubit.get_topo_qubit());
    }
    return ret;
}

unsigned device::Device::get_shortest_cost(unsigned i, unsigned j) const {
    return shortest_cost_[i][j];
}

void device::Device::apply_gate(const Operation& op) {
    Operator gate = op.get_operator();
    std::tuple<unsigned, unsigned> qubits = op.get_qubits();
    device::Qubit& q0 = get_qubit(std::get<0>(qubits));
    device::Qubit& q1 = get_qubit(std::get<1>(qubits));
    unsigned t = op.get_op_time();

    switch (gate) {
        case Operator::Swap: {  // swap topo qubit
            assert(t + SWAP_CYCLE == op.get_cost());

            unsigned temp = q0.get_topo_qubit();
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

        default: {
            assert(false);
        }
    }
}

std::ostream& device::operator<<(std::ostream& os, const device::Qubit& q) {
    return os << "Q" << q.get_id() << ": topo qubit: " << q.get_topo_qubit()
              << " occupied until: " << q.get_avail_time();
}

void device::Device::print_device_state(std::ostream& out) {
    for (unsigned i = 0; i < qubits_.size(); ++i) {
        out << qubits_[i] << "\n";
    }
    out << "\n";
}
