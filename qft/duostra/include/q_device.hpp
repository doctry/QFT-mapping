#pragma once

#include "apsp.hpp"
#include "json.hpp"
#include "operator.hpp"
#include "util.hpp"
#include <assert.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits.h>
#include <queue>
#include <string>
#include <tuple>
#include <vector>
using nlohmann::json;

namespace device {
class Operation {
  public:
    friend std::ostream &operator<<(std::ostream &, Operation &);
    friend std::ostream &operator<<(std::ostream &, const Operation &);
    friend void to_json(json &j, const Operation &op);
    Operation(Operator oper, std::tuple<unsigned, unsigned> qs,
              std::tuple<unsigned, unsigned> du)
        : _oper(oper), _qubits(qs), _duration(du) {
        // sort qs
        unsigned a = std::get<0>(qs);
        unsigned b = std::get<1>(qs);
        assert(a != b);
        if (a > b) {
            _qubits = std::make_tuple(b, a);
        }
    }
    Operation(const Operation &other)
        : _oper(other._oper), _qubits(other._qubits),
          _duration(other._duration) {}

    Operation &operator=(const Operation &other) {
        _oper = other._oper;
        _qubits = other._qubits;
        _duration = other._duration;
        return *this;
    }

    unsigned get_cost() const { return std::get<1>(_duration); }
    unsigned get_op_time() const { return std::get<0>(_duration); }
    std::tuple<unsigned, unsigned> get_duration() const { return _duration; }
    Operator get_operator() const { return _oper; }
    std::string get_operator_name() const {
        switch (_oper) {
        case Operator::Single:
            return "Single";
        case Operator::Swap:
            return "Swap";
        case Operator::CX:
            return "CX";
        default:
            return "Error";
        }
    }
    std::tuple<unsigned, unsigned> get_qubits() const { return _qubits; }

  private:
    Operator _oper;
    std::tuple<unsigned, unsigned> _qubits;
    std::tuple<unsigned, unsigned> _duration; // <from, to>
};

std::ostream &operator<<(std::ostream &, Operation &);
std::ostream &operator<<(std::ostream &, const Operation &);
void to_json(json &j, const Operation &op);

bool op_order(const Operation &a, const Operation &b);
class AStarNode {
  public:
    friend class AStarComp;
    AStarNode(unsigned cost, unsigned id, bool swtch)
        : _estimated_cost(cost), _id(id), _swtch(swtch) {}
    AStarNode(const AStarNode &other)
        : _estimated_cost(other._estimated_cost), _id(other._id),
          _swtch(other._swtch) {}

    bool get_swtch() const { return _swtch; }
    unsigned get_id() const { return _id; }
    unsigned get_cost() const { return _estimated_cost; }

  private:
    unsigned _estimated_cost;
    unsigned _id;
    bool _swtch; // false q0 propagate, true q1 propagate
};

class AStarComp {
  public:
    bool operator()(const AStarNode &a, const AStarNode &b) {
        return a._estimated_cost > b._estimated_cost;
    }
};

class Qubit {
  public:
    friend std::ostream &operator<<(std::ostream &os, const device::Qubit &q);
    Qubit(const unsigned i);
    Qubit(const Qubit &other);
    Qubit(Qubit &&other);

    unsigned get_id() const;
    unsigned get_avail_time() const;
    bool is_adj(Qubit &other) const;
    unsigned get_topo_qubit() const;

    void add_adj(unsigned i);
    void set_topo_qubit(const unsigned i);
    void set_occupied_time(const unsigned t);
    const std::vector<unsigned> &get_adj_list() const;

    // A*
    unsigned get_cost() const;
    bool is_marked() const;
    bool is_taken() const;
    bool get_swtch() const;
    unsigned get_pred() const;
    unsigned get_swap_time() const;
    void mark(bool swtch, unsigned pred);
    void reset();
    void take_route(unsigned cost, unsigned swap_time);

  private:
    unsigned _id;
    std::vector<unsigned> _adj_list;
    unsigned _topo_qubit;
    unsigned _occupied_until;

    // for A*
    bool _marked;
    unsigned _pred;
    unsigned _cost;
    unsigned _swap_time;
    bool _swtch;
    bool _taken;
};

std::ostream &operator<<(std::ostream &os, const device::Qubit &q);

class Device {
  public:
    Device(std::fstream &file, unsigned r, unsigned s, unsigned cx);
    Device(std::vector<std::vector<unsigned>> &, unsigned r, unsigned s,
           unsigned cx);
    Device(const Device &other);
    Device(Device &&other);

    unsigned get_num_qubits() const;
    std::vector<unsigned> mapping() const;
    Qubit &get_qubit(const unsigned i);
    Operation execute_single(Operator op, unsigned q);
    std::vector<Operation>
    routing(Operator op, std::tuple<unsigned, unsigned> qs, bool orient);

    void print_device_state(std::ostream &out);
    void place(std::vector<unsigned> &assign);
    std::vector<Operation> &get_operations();
    std::vector<std::vector<unsigned>> init_apsp();
    void reset();

  private:
    // A*
    std::tuple<bool, unsigned> touch_adj(
        device::Qubit &qubit,
        std::priority_queue<device::AStarNode, std::vector<device::AStarNode>,
                            device::AStarComp> &pq,
        bool swtch); // return <if touch target, target id>, swtch: false q0
                     // propagate, true q1 propagate
    std::vector<Operation> traceback(Operator op, device::Qubit &q0,
                                     device::Qubit &q1, device::Qubit &t0,
                                     device::Qubit &t1); // standalone
    void apply_gate(const Operation &op);

    // data member
    std::vector<Qubit> _qubits;
    unsigned _SINGLE_CYCLE, _SWAP_CYCLE, _CX_CYCLE;
};
} // namespace device
