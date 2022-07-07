#include "algo.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <tuple>
#include <unordered_set>
#include <vector>
#include "limits.h"
#include "tqdm.hpp"
using namespace std;

namespace topo {
bool Gate::is_avail() const {
    bool avail = true;
    for (size_t i = 0; i < _prevs.size(); i++) {
        if (!_prevs[i].second) {
            avail = false;
            break;
        }
    }
    return avail;
}

bool Gate::is_first() const {
    for (auto& elem : _prevs) {
        if (elem.first != UINT_MAX) {
            return false;
        }
    }

    return true;
}

bool Gate::is_last() const {
    for (auto& elem : _nexts) {
        if (elem != UINT_MAX) {
            return false;
        }
    }

    return true;
}

void Gate::finished(unsigned prev) {
#ifdef DEBUG
    bool check = false;
    for (size_t i = 0; i < _prevs.size(); i++) {
        if (_prevs[i].first == prev)
            check = true;
    }
    assert(check);
#endif
    for (size_t i = 0; i < _prevs.size(); i++) {
        if (_prevs[i].first == prev) {
            _prevs[i].second = true;
        }
    }
}

using namespace std;

vector<unsigned> Topology::get_first_gates() const {
    vector<unsigned> result = {};

    for (unsigned i = 0; i < get_num_gates(); ++i) {
        const Gate& gate = get_gate(i);

        if (gate.is_first()) {
            result.push_back(i);
        }
    }

    return result;
}

vector<unsigned> Topology::get_last_gates() const {
    vector<unsigned> result = {};

    for (unsigned i = 0; i < get_num_gates(); ++i) {
        const Gate& gate = get_gate(i);

        if (gate.is_last()) {
            result.push_back(i);
        }
    }

    return result;
}

unordered_map<unsigned, unsigned> Topology::dist_to_first() const {
    unordered_map<unsigned, unsigned> map = {};

    unordered_set<unsigned> current_gen = {};
    for (auto idx : get_first_gates()) {
        map[idx] = 0;
        current_gen.insert(idx);
    }

    cout << "Adding gate\n";
    {
        Tqdm bar{get_num_gates()};
        for (unsigned gen = 0, num_gates = get_num_gates();
             map.size() < num_gates; ++gen) {
            unordered_set<unsigned> next = {};

            for (auto idx : current_gen) {
                for (auto child : get_gate(idx).get_nexts()) {
                    bar.add();
                    next.insert(child);
                    map[child] = gen + 1;
                }
            }

            current_gen = next;
        }
    }
    return map;
}

unordered_map<unsigned, unsigned> Topology::dist_to_last() const {
    unordered_map<unsigned, unsigned> map = {};

    unordered_set<unsigned> current_gen = {};
    for (auto idx : get_last_gates()) {
        map[idx] = 0;
        current_gen.insert(idx);
    }

    cout << "Adding gate\n";
    {
        Tqdm bar{get_num_gates()};
        for (unsigned gen = 0, num_gates = get_num_gates();
             map.size() < num_gates; ++gen) {
            unordered_set<unsigned> prev = {};

            for (auto idx : current_gen) {
                for (auto parent : get_gate(idx).get_prevs()) {
                    bar.add();
                    prev.insert(parent.first);
                    map[parent.first] = gen + 1;
                }
            }

            current_gen = prev;
        }
    }

    return map;
}

unordered_map<unsigned, vector<unsigned>> Topology::gate_by_dist_to_first()
    const {
    auto dist = dist_to_first();
    cout << "Dist to first done\n";
    return gate_by_generation(dist);
}

unordered_map<unsigned, vector<unsigned>> Topology::gate_by_dist_to_last()
    const {
    auto dist = dist_to_last();
    cout << "Dist to last done\n";
    return gate_by_generation(dist);
}
}  // namespace topo

void topo::AlgoTopology::update_avail_gates(unsigned executed) {
    assert(find(_avail_gates.begin(), _avail_gates.end(), executed) !=
           _avail_gates.end());
    Gate& g_exec = _gates[executed];
    _avail_gates.erase(
        std::remove(_avail_gates.begin(), _avail_gates.end(), executed),
        _avail_gates.end());
    assert(g_exec.get_id() == executed);

    vector<unsigned> nexts = g_exec.get_nexts();

    for (unsigned i = 0; i < nexts.size(); i++) {
        unsigned n = nexts[i];
        _gates[n].finished(executed);
        if (_gates[n].is_avail()) {
            _avail_gates.push_back(n);
        }
    }
}

void topo::AlgoTopology::parse(fstream& qasmFile, bool IBMGate) {
    string str;
    for (int i = 0; i < 6; i++)
        qasmFile >> str;
    _num = stoi(str.substr(str.find("[") + 1, str.size() - str.find("[") - 3));
    vector<pair<unsigned, unsigned>> lastCnotWith;
    pair<unsigned, unsigned> init(-1, 0);
    for (size_t i = 0; i < _num; i++) {
        _last_gate.push_back(-1);
        lastCnotWith.push_back(init);
    }
    unsigned gateId = 0;
    vector<string> singleList{"x", "sx", "s", "rz", "i"};
    if (!IBMGate){
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

                unsigned q = stoul(singleQubitId);

                tuple<unsigned, unsigned> temp(q, UINT_MAX);
                topo::Gate tempGate(gateId, Operator::Single, temp);
                tempGate.set_prev(_last_gate[q], _last_gate[q]);

                if (_last_gate[q] != (unsigned)-1)
                    _gates[_last_gate[q]].add_next(gateId);

                // update Id
                _last_gate[q] = gateId;
                _gates.push_back(move(tempGate));
                gateId++;
            } else {
                if (type != "creg" && type != "qreg") {
                    if (IBMGate) {
                       cerr << "IBM machine does not support "<< type  << endl; 
                    }
                    else
                        cerr << "Unseen Gate " << type << endl;
                    assert(true);
                    exit(0);
                } else
                    qasmFile >> str;
            }
        } else {
            if((IBMGate) && (isCRZ == "crz")){
                cerr << "IBM machine does not support crz" << endl;
                assert(true);
                exit(0);
            }
            qasmFile >> str;
            string delimiter = ",";
            string token = str.substr(0, str.find(delimiter));
            string qubitId = token.substr(2, token.size() - 3);
            unsigned q1 = stoul(qubitId);
            token = str.substr(str.find(delimiter) + 1,
                               str.size() - str.find(delimiter) - 2);
            qubitId = token.substr(2, token.size() - 3);
            unsigned q2 = stoul(qubitId);

            tuple<unsigned, unsigned> temp(q1, q2);
            topo::Gate tempGate(gateId, Operator::CX, temp);
            tempGate.set_prev(_last_gate[q1], _last_gate[q2]);

            if (_last_gate[q1] != (unsigned)-1) {
                _gates[_last_gate[q1]].add_next(gateId);
            }

            if (_last_gate[q1] != _last_gate[q2] &&
                _last_gate[q2] != (unsigned)-1) {
                _gates[_last_gate[q2]].add_next(gateId);
            }

            // update Id
            _last_gate[q1] = gateId;
            _last_gate[q2] = gateId;
            _gates.push_back(move(tempGate));
            gateId++;
        }
    }
    for (size_t i = 0; i < _gates.size(); i++) {
        if (_gates[i].is_avail())
            _avail_gates.push_back(i);
    }
#ifdef DEBUG
    print_gates_with_next();
    print_gates_with_prev();
#endif
}

void topo::AlgoTopology::print_gates_with_next() {
    cout << "Print successors of each gate" << endl;
    for (size_t i = 0; i < _gates.size(); i++) {
        vector<unsigned> temp = _gates[i].get_nexts();
        cout << _gates[i].get_id() << "(" << _gates[i].get_type() << ") || ";
        for (size_t j = 0; j < temp.size(); j++) {
            cout << temp[j] << " ";
        }
        cout << endl;
    }
}

void topo::AlgoTopology::print_gates_with_prev() {
    cout << "Print predecessors of each gate" << endl;
    for (size_t i = 0; i < _gates.size(); i++) {
        vector<pair<unsigned, bool>> temp = _gates[i].get_prevs();
        cout << _gates[i].get_id() << "(" << _gates[i].get_type() << ") || ";
        for (size_t j = 0; j < temp.size(); j++) {
            cout << temp[j].first << "(" << temp[j].second << ") ";
        }
        cout << endl;
    }
}
