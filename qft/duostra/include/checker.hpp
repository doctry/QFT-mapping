#pragma once

#include "algo.hpp"
#include "q_device.hpp"
#include "qft_topo.hpp"

class Checker {
   public:
    Checker(topo::Topology& topo,
            device::Device& device,
            const std::vector<device::Operation>& ops,
            const std::vector<size_t>& assign)
        : topo_(topo), device_(device), ops_(ops) {
            device_.place(assign);
        }

    size_t get_cycle(device::Operator& op_type) {
        switch (op_type) {
            case Operator::Swap:
                return device_.SWAP_CYCLE;
            case Operator::CX:
                return device_.CX_CYCLE;
            case Operator::Single:
                return device_.SINGLE_CYCLE;
            default:
                std::cerr << "Wrong Type" << std::endl;
                abort();
        }
    }

    void apply_gate(device::Operation& op, device::Qubit& q0) {
        size_t start = std::get<0>(op.get_duration());
        size_t end = std::get<1>(op.get_duration());

        if (!(start >= q0.get_avail_time())) {
            std::cerr << op << "\n"
                      << "Q" << q0.get_id() << " occu: " << q0.get_avail_time()
                      << std::endl;
            abort();
        }
        if (!(end == start + get_cycle(op.get_operator()))) {
            std::cerr << op << std::endl;
            abort();
        }
        q0.set_occupied_time(end);
    }

    void apply_gate(device::Operation& op,
                    device::Qubit& q0,
                    device::Qubit& q1) {
        size_t start = std::get<0>(op.get_duration());
        size_t end = std::get<1>(op.get_duration());

        if (!(start >= q0.get_avail_time() && start >= q1.get_avail_time())) {
            std::cerr << op << "\n"
                      << "Q" << q0.get_id() << " occu: " << q0.get_avail_time()
                      << "\n"
                      << "Q" << q1.get_id() << " occu: " << q1.get_avail_time()
                      << std::endl;
            abort();
        }
        if (!(end == start + get_cycle(op.get_operator()))) {
            std::cerr << op << std::endl;
            abort();
        }
        q0.set_occupied_time(end);
        q1.set_occupied_time(end);
    }

    void apply_Swap(device::Operation& op) {
        if (op.get_operator() != Operator::Swap) {
            std::cerr << op.get_operator_name << " in apply_Swap" << std::endl;
            abort();
        }
        size_t q0_idx = std::get<0>(op.get_qubits());
        size_t q1_idx = std::get<1>(op.get_qubits());
        auto& q0 = device_.get_qubit(q0_idx);
        auto& q1 = device_.get_qubit(q1_idx);
        apply_gate(op, q0, q1);

        // swap
        size_t temp = q0.get_topo_qubit();
        q0.set_topo_qubit(q1.get_topo_qubit());
        q1.set_topo_qubit(temp);
    }

    bool apply_CX(device::Operation& op, topo::Gate& gate) {
        if (!(op.get_operator() == Operator::CX)) {
            std::cerr << op.get_operator_name() << " in apply_CX" << std::endl;
            abort();
        }
        size_t q0_idx = std::get<0>(op.get_qubits());
        size_t q1_idx = std::get<1>(op.get_qubits());
        auto& q0 = device_.get_qubit(q0_idx);
        auto& q1 = device_.get_qubit(q1_idx);

        size_t topo_0 = q0.get_topo_qubit();
        if (topo_0 == ERROR_CODE) {
            std::cerr << "topo_0 is ERROR CODE" << std::endl;
            abort();
        }
        size_t topo_1 = q1.get_topo_qubit();
        if (topo_1 == ERROR_CODE) {
            std::cerr << "topo_1 is ERRORCODE" << std::endl;
            abort();
        }

        if (topo_0 > topo_1) {
            swap(topo_0, topo_1);
        } else if (topo_0 == topo_1) {
            std::cerr "topo_0 == topo_1: " << topo_0 << std::endl;
            abort();
        }
        if (topo_0 != std::get<0>(gate.get_qubits()) ||
            topo_1 != std::get<1>(gate.get_qubits())) {
            return false;
        }

        apply_gate(op, q0, q1);
        return true;
    }

    bool apply_Single(device::Operation& op, topo::Gate& gate) {
        if (!(op.get_operator() == Operator::Single)) {
            std::cerr << op.get_operator_name() << " in apply_Single"
                      << std::endl;
            abort();
        }
        size_t q0_idx = std::get<0>(op.get_qubits());
        if (std::get<1>(op.get_qubits()) != ERROR_CODE) {
            std::cerr << "Single gate " << gate.get_id()
                      << " has no null second qubit" << std::endl;
            abort();
        }
        auto& q0 = device_.get_qubit(q0_idx);

        size_t topo_0 = q0.get_topo_qubit();
        if (topo_0 == ERROR_CODE) {
            std::cerr << "topo_0 is ERROR CODE" << std::endl;
            abort();
        }

        if (topo_0 != std::get<0>(gate.get_qubits())) {
            return false;
        }

        apply_gate(op, q0);
        return true;
    }

    void test_operations() {
        std::vector<size_t> finished_gates;
        for (auto& op : ops_) {
            if (op.get_operator() == Operator::Swap) {
                apply_Swap(op);
            } else {
                auto& avail_gates = topo_.get_avail_gates();
                bool pass_condition = false;
                if (op.get_operator() == Operator::CX) {
                    for (auto gate : avail_gates) {
                        if (apply_CX(op, topo_.get_gate(gate))) {
                            pass_condition = true;
                            topo_.update_avail_gates(gate);
                            finished_gates.push_back(g);
                        }
                    }
                } else if (op.get_operator() == Operator::Single) {
                    for (auto gate : avail_gates) {
                        if (apply_Single(op, topo_.get_gate(gate))) {
                            pass_condition = true;
                            topo_.update_avail_gates(gate);
                            finished_gates.push_back(g);
                        }
                    }
                } else {
                    std::cerr << "Error gate type " << op.get_operator_name() << std::endl;
                    abort();
                }
                if (!pass_condition) {
                    std::cerr << "Executed gates:\n";
                    for (auto gate : finished_gates) {
                        std::cerr << gate << "\n";
                    }
                    std::cerr << "Available gates:\n";
                    for (auto gate : avail_gates) {
                        std::cerr << gate << "\n";
                    }
                    std::cerr << "Device status:\n";
                    device_.print_device_state();
                    std::cerr << "Failed Operation " << op;
                    abort();
                }
            }

            std::cout << "num gates: " << finished_gates.size() << "\n"
            << "num operations:" << ops_.size() << "\n";
            if(finished_gates.size() != topo_.get_num_gates()) {
                std::cerr << "Number of finished gates " << finished_gates.size() << " different from number of gates" << topo_.get_num_gates() << std::endl;
                abort();
            }
        }
    }

   private:
    topo::Topology& topo_;
    device::Device& device_;
    const std::vector<device::Operation>& ops_;
};