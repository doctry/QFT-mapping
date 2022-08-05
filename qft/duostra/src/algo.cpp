#include "algo.hpp"
#include <algorithm>
#include <climits>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <tuple>
#include <unordered_set>
#include <vector>
#include "tqdm_wrapper.hpp"
#include "util.hpp"

using namespace std;

namespace topo {
void Gate::mark_finished(size_t prev) {
    assert(prevs_->size() == prevs_ok_.size());

    auto found = find(prevs_->begin(), prevs_->end(), prev);
    if (found == prevs_->end()) {
        return;
    }

    size_t idx = found - prevs_->begin();
    prevs_ok_[idx] = true;
}

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

void AlgoTopology::parse(fstream& qasm_file, bool IBM_gate) {
    string str;
    for (int i = 0; i < 6; i++)
        qasm_file >> str;
    num_qubits_ =
        stoi(str.substr(str.find("[") + 1, str.size() - str.find("[") - 3));
    vector<pair<size_t, size_t>> last_cnot_with;
    pair<size_t, size_t> init(-1, 0);
    for (size_t i = 0; i < num_qubits_; i++) {
        last_gate_.push_back(-1);
        last_cnot_with.push_back(init);
    }
    size_t gate_id = 0;
    vector<string> single_list{"x", "sx", "s", "rz", "i"};
    if (!IBM_gate) {
        single_list.push_back("h");
        single_list.push_back("t");
        single_list.push_back("tdg");
    }
    while (qasm_file >> str) {
        string space_delimiter = " ";
        string type = str.substr(0, str.find(" "));
        type = str.substr(0, str.find("("));
        string is_CX = type.substr(0, 2);
        string is_CRZ = type.substr(0, 3);

        if (is_CX != "cx" && is_CRZ != "crz") {
            if (find(begin(single_list), end(single_list), type) !=
                end(single_list)) {
                qasm_file >> str;
                string singleQubitId = str.substr(2, str.size() - 4);

                size_t q = stoul(singleQubitId);

                tuple<size_t, size_t> temp{q, ERROR_CODE};
                Gate temp_gate{gate_id, Operator::Single, temp};
                temp_gate.set_prev(last_gate_[q], last_gate_[q]);

                if (last_gate_[q] != ERROR_CODE) {
                    gates_[last_gate_[q]].add_next(gate_id);
                }

                // update Id
                last_gate_[q] = gate_id;
                gates_.push_back(move(temp_gate));
                ++gate_id;
            } else {
                if (type != "creg" && type != "qreg") {
                    if (IBM_gate) {
                        cerr << "IBM machine does not support " << type << endl;
                    } else {
                        cerr << "Unseen Gate " << type << endl;
                    }
                    assert(true);
                    exit(0);
                } else {
                    qasm_file >> str;
                }
            }
        } else {
            if ((IBM_gate) && (is_CRZ == "crz")) {
                cerr << "IBM machine does not support crz" << endl;
                assert(true);
                exit(0);
            }
            qasm_file >> str;
            string delimiter = ",";
            string token = str.substr(0, str.find(delimiter));
            string qubit_id = token.substr(2, token.size() - 3);
            size_t q1 = stoul(qubit_id);
            token = str.substr(str.find(delimiter) + 1,
                               str.size() - str.find(delimiter) - 2);
            qubit_id = token.substr(2, token.size() - 3);
            size_t q2 = stoul(qubit_id);

            tuple<size_t, size_t> temp{q1, q2};
            Gate temp_gate{gate_id, Operator::CX, temp};
            temp_gate.set_prev(last_gate_[q1], last_gate_[q2]);

            if (last_gate_[q1] != ERROR_CODE) {
                gates_[last_gate_[q1]].add_next(gate_id);
            }

            if (last_gate_[q1] != last_gate_[q2] &&
                last_gate_[q2] != ERROR_CODE) {
                gates_[last_gate_[q2]].add_next(gate_id);
            }

            // update Id
            last_gate_[q1] = gate_id;
            last_gate_[q2] = gate_id;
            gates_.push_back(move(temp_gate));
            ++gate_id;
        }
    }
    for (size_t i = 0; i < gates_.size(); i++) {
        if (gates_[i].is_avail()) {
            avail_gates_.push_back(i);
        }
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
        const auto& prevs = gates_[i].get_prevs();
        const auto& prevs_ok = gates_[i].get_prevs_ok();
        cout << gates_[i].get_id() << "(" << gates_[i].get_type() << ") || ";

        for (size_t j = 0; j < prevs.size(); j++) {
            cout << prevs[j] << "(" << prevs_ok[j] << ") ";
        }
        cout << endl;
    }
}

}  // namespace topo
