#include "qft_placer.hpp"

#include <random>

using namespace placer;
using namespace std;

vector<size_t> DFS::place(Device& device) const {
    vector<size_t> assign;
    vector<bool> qubit_mark(device.get_num_qubits(), false);
    dfs_device(0, device, assign, qubit_mark);
    assert(assign.size() == device.get_num_qubits());
    return assign;
}

void DFS::dfs_device(size_t current,
                     Device& device,
                     vector<size_t>& assign,
                     vector<bool>& qubit_mark) const {
    if (qubit_mark[current]) {
        cout << current << endl;
    }
    assert(!qubit_mark[current]);
    qubit_mark[current] = true;
    assign.push_back(current);

    Qubit& q = device.get_qubit(current);
    vector<size_t> adj_waitlist;

    for (size_t i = 0; i < q.get_adj_list().size(); ++i) {
        size_t adj = q.get_adj_list()[i];

        // already marked
        if (qubit_mark[adj]) {
            continue;
        }

        Qubit& adj_q = device.get_qubit(adj);
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
