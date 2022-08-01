#include "algo.hpp"
#include <climits>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <tuple>
#include <unordered_set>
#include <vector>
#include "tqdm.hpp"
#include "util.hpp"

using namespace std;

namespace topo {

void Gate::set_prev(size_t a, size_t b) {
    pair<size_t, bool> first(a, false);
    pair<size_t, bool> second(b, false);

    if (a != ERROR_CODE) {
        prevs_.push_back(first);
    }

    if (a != b && b != ERROR_CODE) {
        prevs_.push_back(second);
    }
}

bool Gate::is_avail() const {
    for (auto [_, done] : prevs_) {
        if (!done) {
            return false;
        }
    }
    return true;
}

bool Gate::is_first() const {
    for (auto& [idx, _] : prevs_) {
        if (idx != ERROR_CODE) {
            return false;
        }
    }

    return true;
}

bool Gate::is_last() const {
    for (auto& elem : nexts_) {
        if (elem != ERROR_CODE) {
            return false;
        }
    }

    return true;
}

void Gate::finished(size_t prev) {
#ifdef DEBUG
    bool check = false;
    for (size_t i = 0; i < prevs_.size(); i++) {
        if (prevs_[i].first == prev)
            check = true;
    }
    assert(check);
#endif
    for (auto& [idx, done] : prevs_) {
        if (idx == prev) {
            done = true;
        }
    }
}

using namespace std;

unordered_map<size_t, vector<size_t>> Topology::gate_by_generation(
    const unordered_map<size_t, size_t>& map) {
    unordered_map<size_t, vector<size_t>> gen_map;

    for (auto [gate_id, generation] : map) {
        gen_map[generation].push_back(gate_id);
    }

    size_t count = 0;
    for (const auto& [_, gen_ids] : gen_map) {
        count += gen_ids.size();
    }
    assert(count == map.size() &&
           "Resulting map doesn't have the same size as original.");

    return gen_map;
}

void AlgoTopology::update_avail_gates(size_t executed) {
#ifdef DEBUG
    cout << "Available gates: " << avail_gates_;
    cout << ", Executed: " << executed << "\n";
#endif
    assert(find(avail_gates_.begin(), avail_gates_.end(), executed) !=
           avail_gates_.end());
    const Gate& g_exec = gates_[executed];
    avail_gates_.erase(
        std::remove(avail_gates_.begin(), avail_gates_.end(), executed),
        avail_gates_.end());
    assert(g_exec.get_id() == executed);

    vector<size_t> nexts{g_exec.get_nexts()};

    for (size_t i = 0; i < nexts.size(); i++) {
        size_t n = nexts[i];
        gates_[n].finished(executed);
        if (gates_[n].is_avail()) {
            avail_gates_.push_back(n);
        }
    }
}

void AlgoTopology::parse(fstream& qasmFile, bool IBMGate) {
    string str;
    for (int i = 0; i < 6; i++)
        qasmFile >> str;
    num_qubits_ =
        stoi(str.substr(str.find("[") + 1, str.size() - str.find("[") - 3));
    vector<pair<size_t, size_t>> lastCnotWith;
    pair<size_t, size_t> init(-1, 0);
    for (size_t i = 0; i < num_qubits_; i++) {
        last_gate_.push_back(-1);
        lastCnotWith.push_back(init);
    }
    size_t gateId = 0;
    vector<string> singleList{"x", "sx", "s", "rz", "i"};
    if (!IBMGate) {
        singleList.push_back("h");
        singleList.push_back("t");
        singleList.push_back("tdg");
    }
    while (qasmFile >> str) {
        string space_delimiter = " ";
        string type = str.substr(0, str.find(" "));
        type = str.substr(0, str.find("("));
        string isCX = type.substr(0, 2);
        string isCRZ = type.substr(0, 3);

        if (isCX != "cx" && isCRZ != "crz") {
            if (find(begin(singleList), end(singleList), type) !=
                end(singleList)) {
                qasmFile >> str;
                string singleQubitId = str.substr(2, str.size() - 4);

                size_t q = stoul(singleQubitId);

                tuple<size_t, size_t> temp(q, ERROR_CODE);
                Gate tempGate(gateId, Operator::Single, temp);
                tempGate.set_prev(last_gate_[q], last_gate_[q]);

                if (last_gate_[q] != ERROR_CODE)
                    gates_[last_gate_[q]].add_next(gateId);

                // update Id
                last_gate_[q] = gateId;
                gates_.push_back(move(tempGate));
                gateId++;
            } else {
                if (type != "creg" && type != "qreg") {
                    if (IBMGate) {
                        cerr << "IBM machine does not support " << type << endl;
                    } else
                        cerr << "Unseen Gate " << type << endl;
                    assert(true);
                    exit(0);
                } else
                    qasmFile >> str;
            }
        } else {
            if ((IBMGate) && (isCRZ == "crz")) {
                cerr << "IBM machine does not support crz" << endl;
                assert(true);
                exit(0);
            }
            qasmFile >> str;
            string delimiter = ",";
            string token = str.substr(0, str.find(delimiter));
            string qubitId = token.substr(2, token.size() - 3);
            size_t q1 = stoul(qubitId);
            token = str.substr(str.find(delimiter) + 1,
                               str.size() - str.find(delimiter) - 2);
            qubitId = token.substr(2, token.size() - 3);
            size_t q2 = stoul(qubitId);

            tuple<size_t, size_t> temp(q1, q2);
            Gate tempGate(gateId, Operator::CX, temp);
            tempGate.set_prev(last_gate_[q1], last_gate_[q2]);

            if (last_gate_[q1] != ERROR_CODE) {
                gates_[last_gate_[q1]].add_next(gateId);
            }

            if (last_gate_[q1] != last_gate_[q2] &&
                last_gate_[q2] != ERROR_CODE) {
                gates_[last_gate_[q2]].add_next(gateId);
            }

            // update Id
            last_gate_[q1] = gateId;
            last_gate_[q2] = gateId;
            gates_.push_back(move(tempGate));
            gateId++;
        }
    }
    for (size_t i = 0; i < gates_.size(); i++) {
        if (gates_[i].is_avail())
            avail_gates_.push_back(i);
    }
#ifdef DEBUG
    print_gates_with_next();
    print_gates_with_prev();
#endif
}

void AlgoTopology::print_gates_with_next() {
    cout << "Print successors of each gate" << endl;
    for (size_t i = 0; i < gates_.size(); i++) {
        vector<size_t> temp = gates_[i].get_nexts();
        cout << gates_[i].get_id() << "(" << gates_[i].get_type() << ") || ";
        for (size_t j = 0; j < temp.size(); j++) {
            cout << temp[j] << " ";
        }
        cout << endl;
    }
}

void AlgoTopology::print_gates_with_prev() {
    cout << "Print predecessors of each gate" << endl;
    for (size_t i = 0; i < gates_.size(); i++) {
        vector<pair<size_t, bool>> temp = gates_[i].get_prevs();
        cout << gates_[i].get_id() << "(" << gates_[i].get_type() << ") || ";
        for (size_t j = 0; j < temp.size(); j++) {
            cout << temp[j].first << "(" << temp[j].second << ") ";
        }
        cout << endl;
    }
}

}  // namespace topo
