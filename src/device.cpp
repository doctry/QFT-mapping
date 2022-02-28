#include "device.h"
#include <assert.h>
#include <algorithm>
#include <assert.h>
#include <algorithm>

device::Qubit::Qubit(const unsigned i) : _id(i), _occupied_until(0), _marked(false) {}
device::Qubit::Qubit(Qubit &&other) : _id(other._id), _adj_list(std::move(other._adj_list)), _occupied_until(other._occupied_until), _marked(other._marked) {}

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

const unsigned device::Qubit::get_pred() const
{
    return _pred;
}

const unsigned device::Qubit::get_swap_time() const
{
    return _swap_time;
}

void device::Qubit::mark()
{
    assert(_marked == false);
    _marked = true;
}

void device::Qubit::reset()
{
    _marked = false;
}

void device::Qubit::take_route(unsigned cost, unsigned pred, unsigned swap_time)
{
    _cost = cost;
    _pred = pred;
    _swap_time = swap_time;
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
device::Device::Device(Device &&other) : _qubits(std::move(other._qubits)) {}

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
    device::Qubit &t0 = get_qubit(std::get<0>(qs)); // target 0
    device::Qubit &t1 = get_qubit(std::get<1>(qs)); // target 1

    unsigned q0_idx = std::get<0>(qs); // source 0
    unsigned q1_idx = std::get<1>(qs); // source 1

    // priority queue
    std::priority_queue<device::AStarNode, std::vector<device::AStarNode>, device::AStarComp> pq;

    bool swtch = false; // false q0 propagate, true q1 propagate

    push_queue(get_qubit(q0_idx), t1, pq, false);
    push_queue(get_qubit(q1_idx), t0, pq, true);

    while (!get_qubit(q0_idx).is_adj(get_qubit(q1_idx))) // not yet adjacent
    {
        const device::AStarNode &next = pq.top();
        if (next.get_swtch() == false) //q0 propagate
        {
            device::Qubit &q0 = get_qubit(q0_idx);
            unsigned q0_next_idx = next.get_id();
            device::Qubit &q0_next = get_qubit(q0_next_idx);

            unsigned op_time = std::max(q0.get_cost(), q0_next.get_avail_time());
            unsigned cost = op_time + SWAP_CYCLE;
            q0_next.take_route(cost, q0_idx, op_time);
            q0_idx = q0_next_idx;
            push_queue(q0_next, t1, pq, false);
        }
        else // q1 propagate
        {
            device::Qubit &q1 = get_qubit(q1_idx);
            unsigned q1_next_idx = next.get_id();
            device::Qubit &q1_next = get_qubit(q1_next_idx);

            unsigned op_time = std::max(q1.get_cost(), q1_next.get_avail_time());
            unsigned cost = op_time + SWAP_CYCLE;
            q1_next.take_route(cost, q0_idx, op_time);
            q1_idx = q1_next_idx;
            push_queue(q1_next, t1, pq, false);
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
        assert(cheker[i] == false);
        checker[i] = true;
#endif
    }
}

void device::Device::push_queue(device::Qubit &qubit, device::Qubit &target, std::priority_queue<device::AStarNode, std::vector<device::AStarNode>, device::AStarComp> &pq, bool swtch)
{
    const std::vector<unsigned> &adj_list = qubit.get_adj_list();
    for (unsigned i = 0; i < adj_list.size(); ++i)
    {
        device::Qubit &adj = _qubits[adj_list[i]];
        if (adj.is_marked()) // already in the queue
        {
            continue;
        }

        unsigned cost = std::max(qubit.get_cost(), adj.get_avail_time()) + SWAP_CYCLE;
        unsigned estimated_cost = _apsp[adj.get_id()][target.get_id()];
        unsigned heuristic_cost = cost + estimated_cost;
        adj.mark();

        pq.push(std::move(device::AStarNode(heuristic_cost, adj.get_id(), swtch)));
    }
}

std::vector<Operation> device::Device::traceback(device::Qubit &q0, device::Qubit &q1, device::Qubit &t0, device::Qubit &t1)
{
    assert(q0.is_adj(q1));
    std::vector<Operation> ops;

    unsigned R_time = std::max(q0.get_cost(), q1.get_cost());
    Operation R_gate(Operator::R, std::make_tuple(q0.get_id(), q1.get_id()), std::make_tuple(R_time, R_time + R_CYCLE));

    q0.set_occupied_time(R_time + R_CYCLE);
    q1.set_occupied_time(R_time + R_CYCLE);

    ops.push_back(std::move(R_gate));

    unsigned trace_0 = q0.get_id();
    unsigned trace_1 = q1.get_id();
    while (trace_0 != t0.get_id()) // trace 0
    {
        device::Qubit &q_trace_0 = get_qubit(trace_0);
        unsigned trace_pred_0 = q_trace_0.get_pred();
        device::Qubit &q_trace_pred_0 = get_qubit(trace_pred_0);

        // swap gate
        assert(std::max(q_trace_pred_0.get_cost(), q_trace_0.get_avail_time()) + SWAP_CYCLE == q_trace_0.get_cost());

        apply_gate(Operator::Swap, q_trace_0, q_trace_pred_0, q_trace_pred_0.get_swap_time(), ops);

        trace_0 = trace_pred_0;
    }
    while (trace_1 != t0.get_id()) // trace 1
    {
        device::Qubit &q_trace_1 = get_qubit(trace_1);
        unsigned trace_pred_1 = q_trace_1.get_pred();
        device::Qubit &q_trace_pred_1 = get_qubit(trace_pred_1);

        // swap gate
        assert(std::max(q_trace_pred_1.get_cost(), q_trace_1.get_avail_time()) + SWAP_CYCLE == q_trace_1.get_cost());

        apply_gate(Operator::Swap, q_trace_1, q_trace_pred_1, q_trace_pred_1.get_swap_time(), ops);

        trace_1 = trace_pred_1;
    }

    std::sort(ops.begin(), ops.end(), op_order);
    return ops;
}

void device::Device::apply_gate(Operator gate, device::Qubit &q0, device::Qubit &q1, unsigned t, std::vector<Operation> &ops)
{
    switch (gate)
    {
    case Operator::Swap:
        // swap topo qubit
        unsigned temp = q0.get_topo_qubit();
        q0.set_topo_qubit(q1.get_topo_qubit());
        q1.set_topo_qubit(temp);
        q0.set_occupied_time(t + SWAP_CYCLE);
        q1.set_occupied_time(t + SWAP_CYCLE);

        Operation swap_gate(Operator::Swap, std::make_tuple(q0.get_id(), q1.get_id()), std::make_tuple(t, t + SWAP_CYCLE));
        ops.push_back(swap_gate);
        break;

    case Operator::R:
        Operation R_gate(Operator::R, std::make_tuple(q0.get_id(), q1.get_id()), std::make_tuple(t, t + R_CYCLE));
        break;
    default:
        assert(false);
    }
}