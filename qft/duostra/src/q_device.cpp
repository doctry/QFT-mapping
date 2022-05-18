#include "q_device.h"
#include <assert.h>
#include <algorithm>
#include <assert.h>
#include <algorithm>

std::ostream &operator<<(std::ostream &os, Operation &op)
{
    os << std::left;
    unsigned from = std::get<0>(op._duration);
    unsigned to = std::get<1>(op._duration);
    switch (op._oper)
    {
    case Operator::Swap:
        os << std::setw(20) << "Operation: Swap";
        break;
    case Operator::R:
        os << std::setw(20) << "Operation: R";
        break;
    default:
        assert(0);
        break;
    }
    os << "Q" << std::get<0>(op._qubits) << " Q" << std::get<1>(op._qubits) << "    from: " << std::left << std::setw(10) << from << "to: " << to;
    return os;
}

void to_json(json &j, const Operation &op)
{
    std::string oper;
    switch (op._oper)
    {
    case Operator::R:
    {
        oper = "R";
        break;
    }
    case Operator::Swap:
    {
        oper = "Swap";
        break;
    }
    default:
    {
        assert(false);
    }
    }

    j["Operator"] = oper;
    j["Qubits"] = {std::get<0>(op._qubits), std::get<1>(op._qubits)};
    j["duration"] = {std::get<0>(op._duration), std::get<1>(op._duration)};
}

device::Qubit::Qubit(const unsigned i) : _id(i), _topo_qubit(UINT_MAX), _occupied_until(0), _marked(false), _cost(0), _taken(false) {}
device::Qubit::Qubit(Qubit &&other) : _id(other._id), _adj_list(std::move(other._adj_list)), _topo_qubit(other._topo_qubit), _occupied_until(other._occupied_until), _marked(other._marked), _cost(other._cost), _taken(other._taken) {}

void device::Qubit::add_adj(unsigned i)
{
    _adj_list.push_back(i);
}

const unsigned device::Qubit::get_id() const
{
    return _id;
}

const unsigned device::Qubit::get_avail_time() const
{
    return _occupied_until;
}

const bool device::Qubit::is_adj(Qubit &other) const
{
    return !(std::find(_adj_list.begin(), _adj_list.end(), other._id) == _adj_list.end());
}

const unsigned device::Qubit::get_topo_qubit() const
{
    return _topo_qubit;
}

void device::Qubit::set_topo_qubit(const unsigned i)
{
    _topo_qubit = i;
}

void device::Qubit::set_occupied_time(const unsigned t)
{
    if (_occupied_until < t)
    {
        _occupied_until = t;
    }
}

const std::vector<unsigned> &device::Qubit::get_adj_list() const
{
    return _adj_list;
}

const unsigned device::Qubit::get_cost() const
{
    return _cost;
}

const bool device::Qubit::is_marked() const
{
    return _marked;
}

const bool device::Qubit::is_taken() const
{
    return _taken;
}
const bool device::Qubit::get_swtch() const
{
    return _swtch;
}

const unsigned device::Qubit::get_pred() const
{
    return _pred;
}

const unsigned device::Qubit::get_swap_time() const
{
    return _swap_time;
}

void device::Qubit::mark(bool swtch, unsigned pred)
{
    // assert(_marked == false);
    _marked = true;
    _swtch = swtch;
    _pred = pred;
}

void device::Qubit::reset()
{
    _marked = false;
    _taken = false;
    _cost = _occupied_until;
}

void device::Qubit::take_route(unsigned cost, unsigned swap_time)
{
    _cost = cost;
    _swap_time = swap_time;
    _taken = true;
}

std::ostream &operator<<(std::ostream &os, device::Qubit &q)
{
    return os << "Q" << q.get_id() << ": topo qubit: " << q.get_topo_qubit() << " occupied until: " << q.get_avail_time();
}

bool op_order(const Operation &a, const Operation &b)
{
    return a.get_op_time() < b.get_op_time();
}

device::Device::Device(std::fstream &file, unsigned r, unsigned s, unsigned cx) : _R_CYCLE(r), _SWAP_CYCLE(s), _CX_CYCLE(cx)
{
    unsigned num;
    file >> num;

    for (unsigned i = 0; i < num; ++i)
    {
        unsigned id;
        file >> id;
        assert(i == id);
        unsigned num_adj;
        file >> num_adj;

        device::Qubit qubit(i);
        for (unsigned j = 0; j < num_adj; ++j)
        {
            file >> id;
            qubit.add_adj(id);
        }

        _qubits.push_back(std::move(qubit));
    }

    //TODO:apsp
}

device::Device::Device(std::vector<std::vector<unsigned>> &adj_lists, unsigned r, unsigned s, unsigned cx) : _R_CYCLE(r), _SWAP_CYCLE(s), _CX_CYCLE(cx)
{
    for (unsigned i = 0; i < adj_lists.size(); ++i)
    {
        std::vector<unsigned> &adj_list = adj_lists[i];

        device::Qubit qubit(i);
        for (unsigned j = 0; j < adj_list.size(); ++j)
        {
            qubit.add_adj(adj_list[j]);
        }

        _qubits.push_back(std::move(qubit));
    }
}

device::Device::Device(Device &&other) : _qubits(std::move(other._qubits)), _R_CYCLE(other._R_CYCLE), _SWAP_CYCLE(other._SWAP_CYCLE), _ops(std::move(other._ops)) {}

const unsigned device::Device::get_num_qubits() const
{
    return _qubits.size();
}

device::Qubit &device::Device::get_qubit(const unsigned i)
{
    return _qubits[i];
}

std::tuple<std::vector<unsigned>, std::vector<unsigned>> device::Device::route(unsigned source, unsigned target)
{
    unsigned q0_idx = source; // source 0
    unsigned q1_idx = target; // source 1

    if (get_qubit(q0_idx).get_avail_time() > get_qubit(q1_idx).get_avail_time())
    {
        std::swap(q0_idx, q1_idx);
    }

    device::Qubit &t0 = get_qubit(q0_idx); // target 0
    device::Qubit &t1 = get_qubit(q1_idx); // target 1

    // priority queue
    std::priority_queue<device::AStarNode, std::vector<device::AStarNode>, device::AStarComp> pq;

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
    while (!is_adj) // set not adjacent
    {
        device::AStarNode next(pq.top());
        pq.pop();
        unsigned q_next_idx = next.get_id();
        device::Qubit &q_next = get_qubit(q_next_idx);
        assert(q_next.get_swtch() == next.get_swtch());

        unsigned cost = next.get_cost();
        assert(cost >= _SWAP_CYCLE);
        unsigned op_time = cost - _SWAP_CYCLE;
        q_next.take_route(cost, op_time);
        std::tuple<bool, unsigned> touch = touch_adj(q_next, pq, next.get_swtch());
        is_adj = std::get<0>(touch);
        if (is_adj)
        {
            if (next.get_swtch()) // 0 get true means touch 1's set
            {
                q0_idx = std::get<1>(touch);
                q1_idx = q_next_idx;
            }
            else
            {
                q0_idx = q_next_idx;
                q1_idx = std::get<1>(touch);
            }
        }
    }
    return trace(get_qubit(q0_idx), get_qubit(q1_idx), t0, t1);
}

void device::Device::reset()
{
    for (unsigned i = 0; i < _qubits.size(); ++i)
    {
        device::Qubit &qubit = _qubits[i];
        qubit.reset();
    }
}

std::vector<unsigned> device::Device::routing(Operator op, std::tuple<unsigned, unsigned> qs, bool orient)
{
    assert(op == Operator::CX || op == Operator::R);
    unsigned q0_idx = std::get<0>(qs); // source 0
    unsigned q1_idx = std::get<1>(qs); // source 1

    if (orient)
    {
        if (get_qubit(q0_idx).get_avail_time() - get_qubit(q1_idx).get_avail_time() > _SWAP_CYCLE)
        {
            std::swap(q0_idx, q1_idx);
        }
        else if (get_qubit(q1_idx).get_avail_time() - get_qubit(q0_idx).get_avail_time() < _SWAP_CYCLE && get_qubit(q0_idx).get_topo_qubit() > get_qubit(q1_idx).get_topo_qubit())
        {
            std::swap(q0_idx, q1_idx);
        }
    }
    else
    {
        if (get_qubit(q0_idx).get_avail_time() > get_qubit(q1_idx).get_avail_time())
        {
            std::swap(q0_idx, q1_idx);
        }
    }

    device::Qubit &t0 = get_qubit(q0_idx); // target 0
    device::Qubit &t1 = get_qubit(q1_idx); // target 1

    // priority queue
    std::priority_queue<device::AStarNode, std::vector<device::AStarNode>, device::AStarComp> pq;

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

    while (!is_adj) // set not adjacent
    {
        device::AStarNode next(pq.top());
        pq.pop();
        unsigned q_next_idx = next.get_id();
        device::Qubit &q_next = get_qubit(q_next_idx);
        assert(q_next.get_swtch() == next.get_swtch());

        unsigned cost = next.get_cost();
        assert(cost >= _SWAP_CYCLE);
        unsigned op_time = cost - _SWAP_CYCLE;
        q_next.take_route(cost, op_time);
        std::tuple<bool, unsigned> touch = touch_adj(q_next, pq, next.get_swtch());
        is_adj = std::get<0>(touch);
        if (is_adj)
        {
            if (next.get_swtch()) // 0 get true means touch 1's set
            {
                q0_idx = std::get<1>(touch);
                q1_idx = q_next_idx;
            }
            else
            {
                q0_idx = q_next_idx;
                q1_idx = std::get<1>(touch);
            }
        }
    }
    std::vector<Operation> ops = traceback(op, get_qubit(q0_idx), get_qubit(q1_idx), t0, t1);

#ifdef DEBUG
    for (unsigned i = 0; i < ops.size(); ++i)
    {
        std::cout << ops[i] << "\n";
    }
#endif

    _ops.insert(_ops.end(), ops.begin(), ops.end()); // append operations to the back

    // reset and record change
    std::vector<unsigned> rec_change; // <topo qubit, device qubit>
#ifdef DEBUG
    std::vector<bool> checker(_qubits.size(), false);
#endif
    for (unsigned i = 0; i < _qubits.size(); ++i)
    {
        device::Qubit &qubit = _qubits[i];
        qubit.reset();
        assert(qubit.get_topo_qubit() < _qubits.size());
        rec_change.push_back(qubit.get_topo_qubit());
#ifdef DEBUG
        if (i != UINT_MAX)
        {
            assert(checker[i] == false);
            checker[i] = true;
        }
#endif
    }
    return rec_change;
}

std::tuple<bool, unsigned> device::Device::touch_adj(device::Qubit &qubit, std::priority_queue<device::AStarNode, std::vector<device::AStarNode>, device::AStarComp> &pq, bool swtch)
{
    const std::vector<unsigned> &adj_list = qubit.get_adj_list();
    for (unsigned i = 0; i < adj_list.size(); ++i)
    {
        device::Qubit &adj = _qubits[adj_list[i]];
        if (adj.is_marked()) // already in the queue
        {
            if (adj.is_taken())
            {
                if (adj.get_swtch() != swtch) // touch target
                {
                    assert(adj.get_id() == adj_list[i]);
                    return std::make_tuple(true, adj.get_id());
                }
            }
            continue;
        }

        unsigned cost = std::max(qubit.get_cost(), adj.get_avail_time()) + _SWAP_CYCLE;
        unsigned estimated_cost = 0;
        unsigned heuristic_cost = cost + estimated_cost;
        adj.mark(swtch, qubit.get_id());

        pq.push(std::move(device::AStarNode(heuristic_cost, adj.get_id(), swtch)));
    }
    return std::make_tuple(false, UINT_MAX);
}

std::vector<Operation> device::Device::traceback(Operator op, device::Qubit &q0, device::Qubit &q1, device::Qubit &t0, device::Qubit &t1)
{
    assert(t0.get_id() == t0.get_pred());
    assert(t1.get_id() == t1.get_pred());

    assert(q0.is_adj(q1));
    std::vector<Operation> ops;

    unsigned op_time = std::max(q0.get_cost(), q1.get_cost());

    if (op == Operator::R)
    {
        Operation R_gate(Operator::R, std::make_tuple(q0.get_id(), q1.get_id()), std::make_tuple(op_time, op_time + _R_CYCLE));
        ops.push_back(R_gate);
    }
    else if (op == Operator::CX)
    {
        Operation CX_gate(Operator::CX, std::make_tuple(q0.get_id(), q1.get_id()), std::make_tuple(op_time, op_time + _CX_CYCLE));
        ops.push_back(CX_gate);
    }

    unsigned trace_0 = q0.get_id();
    unsigned trace_1 = q1.get_id();
    while (trace_0 != t0.get_id()) // trace 0
    {
        device::Qubit &q_trace_0 = get_qubit(trace_0);
        unsigned trace_pred_0 = q_trace_0.get_pred();

        unsigned swap_time = q_trace_0.get_swap_time();
        Operation swap_gate(Operator::Swap, std::make_tuple(trace_0, trace_pred_0), std::make_tuple(swap_time, swap_time + _SWAP_CYCLE));
        ops.push_back(swap_gate);

        trace_0 = trace_pred_0;
    }
    while (trace_1 != t1.get_id()) // trace 1
    {
        device::Qubit &q_trace_1 = get_qubit(trace_1);
        unsigned trace_pred_1 = q_trace_1.get_pred();

        unsigned swap_time = q_trace_1.get_swap_time();
        Operation swap_gate(Operator::Swap, std::make_tuple(trace_1, trace_pred_1), std::make_tuple(swap_time, swap_time + _SWAP_CYCLE));
        ops.push_back(swap_gate);

        trace_1 = trace_pred_1;
    }

    std::sort(ops.begin(), ops.end(), op_order);
    for (unsigned i = 0; i < ops.size(); ++i)
    {
        apply_gate(ops[i]);
    }

    return ops;
}

std::tuple<std::vector<unsigned>, std::vector<unsigned>> device::Device::trace(device::Qubit &q0, device::Qubit &q1, device::Qubit &t0, device::Qubit &t1)
{
    assert(t0.get_id() == t0.get_pred());
    assert(t1.get_id() == t1.get_pred());

    assert(q0.is_adj(q1));
    std::vector<unsigned> route_0;
    std::vector<unsigned> route_1;

    unsigned trace_0 = q0.get_id();
    route_0.push_back(trace_0);
    unsigned trace_1 = q1.get_id();
    route_1.push_back(trace_1);
    while (trace_0 != t0.get_id()) // trace 0
    {
        device::Qubit &q_trace_0 = get_qubit(trace_0);
        unsigned trace_pred_0 = q_trace_0.get_pred();
        route_0.push_back(trace_pred_0);
        trace_0 = trace_pred_0;
    }
    std::reverse(route_0.begin(), route_0.end());
    assert(route_0[0] == t0.get_id());

    while (trace_1 != t1.get_id()) // trace 1
    {
        device::Qubit &q_trace_1 = get_qubit(trace_1);
        unsigned trace_pred_1 = q_trace_1.get_pred();
        route_1.push_back(trace_pred_1);
        trace_1 = trace_pred_1;
    }
    std::reverse(route_1.begin(), route_1.end());
    assert(route_1[0] == t1.get_id());

    return std::make_tuple(route_0, route_1);
}

void device::Device::place(std::vector<unsigned> &assign)
{
    for (unsigned i = 0; i < assign.size(); ++i)
    {
        device::Qubit &q = _qubits[assign[i]];
        assert(q.get_topo_qubit() == UINT_MAX);
        q.set_topo_qubit(i);
    }
}

std::vector<Operation> device::Device::compile_route(const std::tuple<std::vector<unsigned>, std::vector<unsigned>> &routes)
{
    const std::vector<unsigned> &route_0 = std::get<0>(routes);
    const std::vector<unsigned> &route_1 = std::get<1>(routes);
    device::Qubit &q0 = get_qubit(route_0.back());
    device::Qubit &q1 = get_qubit(route_1.back());
#ifdef DEBUG
    device::Qubit &t0 = get_qubit(route_0[0]);
    device::Qubit &t1 = get_qubit(route_1[0]);
    assert(t0.get_id() == t0.get_pred());
    assert(t1.get_id() == t1.get_pred());
#endif

    assert(q0.is_adj(q1));
    std::vector<Operation> ops;

    unsigned R_time = std::max(q0.get_cost(), q1.get_cost());
    Operation R_gate(Operator::R, std::make_tuple(q0.get_id(), q1.get_id()), std::make_tuple(R_time, R_time + _R_CYCLE));
    ops.push_back(R_gate);

    for (unsigned i = 1; i < route_0.size(); ++i) // trace 0
    {
        unsigned parent = route_0[i - 1];
        unsigned child = route_0[i];
        unsigned swap_time = get_qubit(child).get_swap_time();
        Operation swap_gate(Operator::Swap, std::make_tuple(parent, child), std::make_tuple(swap_time, swap_time + _SWAP_CYCLE));
        ops.push_back(swap_gate);
    }
    for (unsigned i = 1; i < route_1.size(); ++i) // trace 0
    {
        unsigned parent = route_1[i - 1];
        unsigned child = route_1[i];
        unsigned swap_time = get_qubit(child).get_swap_time();
        Operation swap_gate(Operator::Swap, std::make_tuple(parent, child), std::make_tuple(swap_time, swap_time + _SWAP_CYCLE));
        ops.push_back(swap_gate);
    }

    std::sort(ops.begin(), ops.end(), op_order);
    for (unsigned i = 0; i < ops.size(); ++i)
    {
        apply_gate(ops[i]);
    }

    return ops;
}

std::vector<unsigned> device::Device::mapping() const
{
    std::vector<unsigned> ret(_qubits.size(), UINT_MAX);

    for (unsigned i = 0; i < _qubits.size(); ++i)
    {
        unsigned topo_idx = _qubits[i].get_topo_qubit();
        assert(ret[topo_idx] == UINT_MAX);
        ret[topo_idx] = i;
    }
    return ret;
}

void device::Device::apply_gate(const Operation &op)
{
    Operator gate = op.get_operator();
    std::tuple<unsigned, unsigned> qubits = op.get_qubits();
    device::Qubit &q0 = get_qubit(std::get<0>(qubits));
    device::Qubit &q1 = get_qubit(std::get<1>(qubits));
    unsigned t = op.get_op_time();

    switch (gate)
    {
    case Operator::Swap:
    { // swap topo qubit
        assert(t + _SWAP_CYCLE == op.get_cost());

        unsigned temp = q0.get_topo_qubit();
        q0.set_topo_qubit(q1.get_topo_qubit());
        q1.set_topo_qubit(temp);
        q0.set_occupied_time(t + _SWAP_CYCLE);
        q1.set_occupied_time(t + _SWAP_CYCLE);
        break;
    }

    case Operator::R:
    {
        assert(t + _R_CYCLE == op.get_cost());

        q0.set_occupied_time(t + _R_CYCLE);
        q1.set_occupied_time(t + _R_CYCLE);
        break;
    }

    default:
    {
        assert(false);
    }
    }
}

void device::Device::write_assembly(std::ostream &out)
{
    std::sort(_ops.begin(), _ops.end(), op_order);

    for (unsigned i = 0; i < _ops.size(); ++i)
    {
        Operation &op = _ops[i];
        switch (op.get_operator())
        {
        case Operator::R:
            out << "R ";
            break;
        case Operator::Swap:
            out << "Swap ";
            break;
        default:
            assert(false);
        }
        std::tuple<unsigned, unsigned> qubits = op.get_qubits();
        out << "Q[" << std::get<0>(qubits) << "] Q[" << std::get<1>(qubits) << "]; ";
        out << "(" << op.get_op_time() << "," << op.get_cost() << ")\n";
    }
}

void device::Device::to_json(json &j)
{
    std::sort(_ops.begin(), _ops.end(), op_order);

    json o;
    for (unsigned i = 0; i < _ops.size(); ++i)
    {
        Operation &op = _ops[i];
        json buf = op;
        o.push_back(buf);
    }
    j["Operations"] = o;
}

unsigned device::Device::get_final_cost()
{
    std::sort(_ops.begin(), _ops.end(), op_order);

    return _ops[_ops.size() - 1].get_cost();
}

unsigned device::Device::get_total_time()
{
    std::sort(_ops.begin(), _ops.end(), op_order);

    unsigned ret = 0;
    for (unsigned i = 0; i < _ops.size(); ++i)
    {
        std::tuple<unsigned, unsigned> dur = _ops[i].get_duration();
        ret += std::get<1>(dur) - std::get<0>(dur);
    }
    return ret;
}

unsigned device::Device::get_swap_num()
{
    std::sort(_ops.begin(), _ops.end(), op_order);
    unsigned ret = 0;
    for (unsigned i = 0; i < _ops.size(); ++i)
    {
        if (_ops[i].get_operator() == Operator::Swap)
        {
            ret += 1;
        }
    }
    return ret;
}

void device::Device::print_device_state(std::ostream &out)
{
    for (unsigned i = 0; i < _qubits.size(); ++i)
    {
        out << _qubits[i] << "\n";
    }
    out << "\n";
}

std::vector<Operation> &device::Device::get_operations()
{
    return _ops;
}