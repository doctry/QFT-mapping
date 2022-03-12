#include "device.h"
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
        assert(to - from == args.SWAP_CYCLE);
        os << std::setw(20) << "Operation: Swap";
        break;
    case Operator::R:
        assert(to - from == args.R_CYCLE);
        os << std::setw(20) << "Operation: R";
        break;
    default:
        assert(0);
        break;
    }
    os << "Q" << std::get<0>(op._qubits) << " Q" << std::get<1>(op._qubits) << "    from: " << std::left << std::setw(10) << from << "to: " << to;
    return os;
}

bool op_order(const Operation &a, const Operation &b)
{
    return std::get<0>(a._duration) < std::get<0>(b._duration);
}

device::Qubit::Qubit(const unsigned i) : _id(i), _occupied_until(0), _marked(false), _cost(0), _taken(false) {}
device::Qubit::Qubit(Qubit &&other) : _id(other._id), _adj_list(std::move(other._adj_list)), _occupied_until(other._occupied_until), _marked(other._marked), _cost(other._cost), _taken(other._taken) {}

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

device::Device::Device(std::fstream &file)
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
device::Device::Device(Device &&other) : _qubits(std::move(other._qubits)), _ops(std::move(other._ops)) {}

const unsigned device::Device::get_num_qubits() const
{
    return _qubits.size();
}

device::Qubit &device::Device::get_qubit(const unsigned i)
{
    return _qubits[i];
}

std::vector<unsigned> device::Device::routing(std::tuple<unsigned, unsigned> qs)
{
    unsigned q0_idx = std::get<0>(qs); // source 0
    unsigned q1_idx = std::get<1>(qs); // source 1

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
        assert(cost >= args.SWAP_CYCLE);
        unsigned op_time = cost - args.SWAP_CYCLE;
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
    std::vector<Operation> ops = traceback(get_qubit(q0_idx), get_qubit(q1_idx), t0, t1);
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
        assert(checker[i] == false);
        checker[i] = true;
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

        unsigned cost = std::max(qubit.get_cost(), adj.get_avail_time()) + args.SWAP_CYCLE;
        unsigned estimated_cost = 0;
        unsigned heuristic_cost = cost + estimated_cost;
        adj.mark(swtch, qubit.get_id());

        pq.push(std::move(device::AStarNode(heuristic_cost, adj.get_id(), swtch)));
    }
    return std::make_tuple(false, UINT_MAX);
}

std::vector<Operation> device::Device::traceback(device::Qubit &q0, device::Qubit &q1, device::Qubit &t0, device::Qubit &t1)
{
    assert(t0.get_id() == t0.get_pred());
    assert(t1.get_id() == t1.get_pred());

    assert(q0.is_adj(q1));
    std::vector<Operation> ops;

    unsigned R_time = std::max(q0.get_cost(), q1.get_cost());
    Operation R_gate(Operator::R, std::make_tuple(q0.get_id(), q1.get_id()), std::make_tuple(R_time, R_time + args.R_CYCLE));
    ops.push_back(R_gate);

    unsigned trace_0 = q0.get_id();
    unsigned trace_1 = q1.get_id();
    while (trace_0 != t0.get_id()) // trace 0
    {
        device::Qubit &q_trace_0 = get_qubit(trace_0);
        unsigned trace_pred_0 = q_trace_0.get_pred();

        unsigned swap_time = q_trace_0.get_swap_time();
        Operation swap_gate(Operator::Swap, std::make_tuple(trace_0, trace_pred_0), std::make_tuple(swap_time, swap_time + args.SWAP_CYCLE));
        ops.push_back(swap_gate);

        trace_0 = trace_pred_0;
    }
    while (trace_1 != t1.get_id()) // trace 1
    {
        device::Qubit &q_trace_1 = get_qubit(trace_1);
        unsigned trace_pred_1 = q_trace_1.get_pred();
        
        unsigned swap_time = q_trace_1.get_swap_time();
        Operation swap_gate(Operator::Swap, std::make_tuple(trace_1, trace_pred_1), std::make_tuple(swap_time, swap_time + args.SWAP_CYCLE));
        ops.push_back(swap_gate);

        trace_1 = trace_pred_1;
    }

    for (unsigned i = 0; i < ops.size(); ++i) {
        std::sort(ops.begin(), ops.end(), op_order);
        apply_gate(ops[i]);
    }

    return ops;
}

void device::Device::apply_gate(const Operation& op)
{
    Operator gate = op.get_operator();
    std::tuple<unsigned, unsigned> qubits = op.get_qubits();
    device::Qubit& q0 = get_qubit(std::get<0>(qubits));
    device::Qubit& q1 = get_qubit(std::get<1>(qubits));
    unsigned t = op.get_op_time();

    switch (gate)
    {
    case Operator::Swap:
    { // swap topo qubit
        assert (t + args.SWAP_CYCLE == op.get_cost());

        unsigned temp = q0.get_topo_qubit();
        q0.set_topo_qubit(q1.get_topo_qubit());
        q1.set_topo_qubit(temp);
        q0.set_occupied_time(t + args.SWAP_CYCLE);
        q1.set_occupied_time(t + args.SWAP_CYCLE);
        break;
    }

    case Operator::R:
    {
        assert (t + args.R_CYCLE == op.get_cost());

        q0.set_occupied_time(t + args.R_CYCLE);
        q1.set_occupied_time(t + args.R_CYCLE);
        break;
    }

    default:
    {
        assert(false);
    }
    }
}

void device::Device::print_operations(std::ostream &out)
{
    std::sort(_ops.begin(), _ops.end(), op_order);

    for (unsigned i = 0; i < _ops.size(); ++i)
    {
        out << _ops[i] << "\n";
    }
    out << "\n";
}

unsigned device::Device::get_final_cost()
{
    std::sort(_ops.begin(), _ops.end(), op_order);

    return _ops[_ops.size() - 1].get_cost();
}

void device::Device::print_device_state(std::ostream &out)
{
    for (unsigned i = 0; i < _qubits.size(); ++i)
    {
        out << _qubits[i] << "\n";
    }
    out << "\n";
}