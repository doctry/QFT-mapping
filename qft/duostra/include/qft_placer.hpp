#pragma once

#include <random>
#include <vector>
#include "q_device.hpp"

class QFTPlacer {
   public:
    QFTPlacer() {}
    QFTPlacer(const QFTPlacer& other) = delete;
    QFTPlacer(QFTPlacer&& other) = delete;

    std::vector<size_t> place(device::Device& device, std::string& typ) {
        if (typ == "static") {
            return static_place(device);
        } else if (typ == "random") {
            return random_place(device);
        } else if (typ == "dfs") {
            return dfs_place(device);
        } else {
            std::cerr << typ << " is not a placer type." << std::endl;
            abort();
        }
    }

    std::vector<size_t> random_place(device::Device& device) {
        std::vector<size_t> assign;
        for (size_t i = 0; i < device.get_num_qubits(); ++i) {
            assign.push_back(i);
        }
        size_t seed =
            std::chrono::system_clock::now().time_since_epoch().count();

        shuffle(assign.begin(), assign.end(), std::default_random_engine(seed));
        return assign;
    }

    std::vector<size_t> static_place(device::Device& device) {
        std::vector<size_t> assign;
        for (size_t i = 0; i < device.get_num_qubits(); ++i) {
            assign.push_back(i);
        }
        return assign;
    }

    std::vector<size_t> dfs_place(device::Device& device) {
        std::vector<size_t> assign;
        std::vector<bool> qubit_mark(device.get_num_qubits(), false);
        dfs_device(0, device, assign, qubit_mark);
        assert(assign.size() == device.get_num_qubits());
        return assign;
    }

   private:
    inline void dfs_device(size_t current,
                           device::Device& device,
                           std::vector<size_t>& assign,
                           std::vector<bool>& qubit_mark) {
        if (qubit_mark[current] == true) {
            std::cout << current << std::endl;
        }
        assert(qubit_mark[current] == false);
        qubit_mark[current] = true;
        assign.push_back(current);

        device::Qubit& q = device.get_qubit(current);
        std::vector<size_t> adj_waitlist;

        for (size_t i = 0; i < q.get_adj_list().size(); ++i) {
            size_t adj = q.get_adj_list()[i];

            // already marked
            if (qubit_mark[adj]) {
                continue;
            }

            device::Qubit& adj_q = device.get_qubit(adj);
            assert(adj_q.get_adj_list().size() > 0);

            // corner
            if (adj_q.get_adj_list().size() == 1) {
                dfs_device(adj, device, assign, qubit_mark);
            } else {
                adj_waitlist.push_back(adj);
            }
        }
        for (size_t i = 0; i < adj_waitlist.size(); ++i) {
            size_t adj = adj_waitlist[i];
            if (qubit_mark[adj]) {
                continue;
            }
            dfs_device(adj, device, assign, qubit_mark);
        }
        return;
    }
};
