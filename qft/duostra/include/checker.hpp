#pragma once

#include "algo.hpp"
#include "q_device.hpp"
#include "qft_topo.hpp"

class Checker {
   public:
    Checker(topo::Topology& topo,
            device::Device& device,
            std::vector<device::Operation>& ops)
        : topo_(topo), device_(device), ops_(ops) {}

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

    void apply_gate(device::Operation& op,
                    device::Qubit& q0) {
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

    void apply_Swap(self, device::Operation& op) {
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
        swap(device2topo_[q0_idx], device2topo_[q1_idx]);
    }

    bool apply_CX(device::Operation& op, topo::Gate& gate) {
        if(!(op.get_operator() == Operator::CX)){
            std::cerr << op.get_operator_name() << " in apply_CX" << std::endl;
            abort();
        }
        size_t q0_idx = std::get<0>(op.get_qubits());
        size_t q1_idx = std::get<1>(op.get_qubits());
        auto& q0 = device_.get_qubit(q0_idx);
        auto& q1 = device_.get_qubit(q1_idx);

        size_t topo_0 = device2topo_[q0_idx];
        if(topo_0 == ERROR_CODE) {
            std::cerr << "topo_0 is ERROR CODE" << std::endl;
            abort();
        }
        size_t topo_1 = device2topo_[q1_idx];
        if(topo_1 == ERROR_CODE) {
            std::cerr << "topo_1 is ERRORCODE" << std::endl;
            abort();
        }

        if (topo_0 > topo_1) {
            swap(topo_0, topo_1);
        } else if(topo_0 == topo_1) {
            std::cerr "topo_0 == topo_1: " << topo_0 << std::endl;
            abort();
        }
        if(topo_0 != std::get<0>(gate.get_qubits()) || topo_1 != std::get<1>(gate.get_qubits())){
            return false;
        }

        apply_gate(op, q0, q1);
        return true;
    }

    bool apply_Single(device::Operation& op, topo::Gate& gate) {
        if(!(op.get_operator() == Operator::Single)){
            std::cerr << op.get_operator_name() << " in apply_Single" << std::endl;
            abort();
        }
        size_t q0_idx = std::get<0>(op.get_qubits());
        if(std::get<1>(op.get_qubits()) != ERROR_CODE){
            std::cerr << "Single gate " << gate.get_id() << " has no null second qubit" << std::endl;
            abort();
        }
        auto& q0 = device_.get_qubit(q0_idx);

        size_t topo_0 = device2topo_[q0_idx];
        if(topo_0 == ERROR_CODE) {
            std::cerr << "topo_0 is ERROR CODE" << std::endl;
            abort();
        }

        if(topo_0 != std::get<0>(gate.get_qubits())){
            return false;
        }

        apply_gate(op, q0);
        return true;
    }

    size_t test_operations(self) {
        std::vector<size_t> finished_gates;
        for (auto& op: ops_) {
            
        }
        for op in self.operations:
            if op.operator == "Swap":
                self.apply_Swap(op)
            elif op.operator == "CX":
                avail_gates = self.topo.avail_gates
                pass_condition = False
                for g in avail_gates:
                    if self.apply(op, self.topo.gates[g]):
                        pass_condition = True
                        topo.update_avail_gates(g)
                        finished_gates.append(g)
                        break
                if pass_condition == False:
                    print("Executed gates:")
                    for g in finished_gates:
                        print(self.topo.gates[g])
                    print("Available gates:")
                    for g in avail_gates:
                        print(self.topo.gates[g])
                    print("Device status:")
                    print(self.device)
                    print("Failed Operation", op)
                    raise RuntimeError("Operation cannot match device.")

        print("num gates:", len(finished_gates))
        print("num operations:", len(self.operations))
        assert len(finished_gates) == len(self.topo.gates), [
            len(finished_gates),
            len(self.topo.gates),
        ]
        return self.operations[-1].duration[1]
    }

   private:
    topo::Topology& topo_;
    device::Device& device_;
    std::vector<device::Operation>& ops_;
    std::vector<size_t> device2topo_;
};