#include "qft_scheduler.hpp"

#include <algorithm>
#include <cassert>

using namespace scheduler;

SchedulerBase::SchedulerBase(unique_ptr<Topology> topo) noexcept
    : topo_(move(topo)) {}

SchedulerBase::SchedulerBase(const SchedulerBase& other) noexcept
    : topo_(other.topo_->clone()) {}

SchedulerBase::SchedulerBase(SchedulerBase&& other) noexcept
    : topo_(move(other.topo_)) {}

unique_ptr<SchedulerBase> SchedulerBase::clone() const {
    return make_unique<SchedulerBase>(*this);
}

void SchedulerBase::sort() {
    std::sort(ops_.begin(), ops_.end(), device::op_order);
    sorted_ = true;
}

void SchedulerBase::write_assembly(ostream& out) const {
    assert(sorted_);

    for (size_t i = 0; i < ops_.size(); ++i) {
        const auto& op = ops_.at(i);
        auto operator_name = operator_get_name(op.get_operator());
        out << operator_name << " ";
        tuple<size_t, size_t> qubits = op.get_qubits();
        out << "Q[" << std::get<0>(qubits) << "] Q[" << std::get<1>(qubits)
            << "]; ";
        out << "(" << op.get_op_time() << "," << op.get_cost() << ")\n";
    }
}

void SchedulerBase::to_json(json& j) const {
    assert(sorted_);

    json o;
    for (size_t i = 0; i < ops_.size(); ++i) {
        const auto& op = ops_.at(i);
        json buf = op;
        o.push_back(buf);
    }
    j["Operations"] = o;
}

void SchedulerBase::assign_gates(unique_ptr<QFTRouter> router) {
    cout << "Default scheduler running..." << endl;

    Tqdm bar{int(topo_->get_num_gates())};
    for (size_t i = 0; i < topo_->get_num_gates(); ++i) {
        bar.add();
        route_one_gate(*router, i);
    }
}

size_t SchedulerBase::get_final_cost() const {
    assert(sorted_);
    return ops_.at(ops_.size() - 1).get_cost();
}

size_t SchedulerBase::get_total_time() const {
    assert(sorted_);

    size_t ret = 0;
    for (size_t i = 0; i < ops_.size(); ++i) {
        tuple<size_t, size_t> dur = ops_[i].get_duration();
        ret += std::get<1>(dur) - std::get<0>(dur);
    }
    return ret;
}

size_t SchedulerBase::get_swap_num() const {
    size_t ret = 0;
    for (size_t i = 0; i < ops_.size(); ++i) {
        if (ops_.at(i).get_operator() == Operator::Swap) {
            ++ret;
        }
    }
    return ret;
}

const vector<size_t>& SchedulerBase::get_avail_gates() const {
    return topo_->get_avail_gates();
}

const vector<device::Operation>& SchedulerBase::get_operations() const {
    return ops_;
}

size_t SchedulerBase::ops_cost() const {
    return std::get<1>(
        std::max_element(ops_.begin(), ops_.end(), [](auto a, auto b) {
            return std::get<1>(a.get_duration()) <
                   std::get<1>(b.get_duration());
        })->get_duration());
}

size_t SchedulerBase::get_executable(QFTRouter& router,
                                       vector<size_t> wait_list) const {
    for (size_t gate_idx : wait_list) {
        if (router.is_executable(topo_->get_gate(gate_idx))) {
            return gate_idx;
        }
    }
    return size_t(-1);
}

void SchedulerBase::route_one_gate(QFTRouter& router, size_t gate_idx) {
    const auto& gate = topo_->get_gate(gate_idx);
    auto ops{router.assign_gate(gate)};
    ops_.insert(ops_.end(), ops.begin(), ops.end());
    topo_->update_avail_gates(gate_idx);
}
