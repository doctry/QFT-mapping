#include "algo.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <tuple>
#include <vector>
using namespace std;

bool topo::Gate::is_avail() {
    bool avail = true;
    for (size_t i = 0; i < _prevs.size(); i++) {
        if (!_prevs[i].second) {
            avail = false;
            break;
        }
    }
    return avail;
}

void topo::Gate::finished(unsigned prev) {
#ifdef DEBUG
    bool check = false;
    for (size_t i = 0; i < _prevs.size(); i++) {
        if (_prevs[i].first == prev)
            check = true;
    }
    assert(check);
#endif
    for (size_t i = 0; i < _prevs.size(); i++) {
        if (_prevs[i].first == prev)
            _prevs[i].second = true;
    }
}

void topo::AlgoTopology::update_avail_gates(unsigned executed) {
    assert(find(_avail_gates.begin(), _avail_gates.end(), executed) !=
           _avail_gates.end());
    Gate &g_exec = _gates[executed];
    _avail_gates.erase(
        std::remove(_avail_gates.begin(), _avail_gates.end(), executed),
        _avail_gates.end());
    assert(g_exec.get_id() == executed);

    vector<unsigned> nexts = g_exec.get_nexts();

    for (unsigned i = 0; i < nexts.size(); i++) {
        unsigned n = nexts[i];
        _gates[n].finished(executed);
        if (_gates[n].is_avail())
            _avail_gates.push_back(n);
    }
}

void topo::AlgoTopology::parse(fstream &qasmFile) {
    string str;
    for (int i = 0; i < 6; i++)
        qasmFile >> str;
    _num = stoi(str.substr(str.find("[") + 1, str.size() - str.find("[") - 3));
    vector<pair<unsigned, unsigned>> lastCnotWith;
    pair<unsigned, unsigned> init(-1, 0);
    for (size_t i = 0; i < _num; i++) {
        _lastGate.push_back(-1);
        lastCnotWith.push_back(init);
    }
    unsigned gateId = 0;
    vector<string> singleList{"h", "t", "x", "tdg", "sx", "s", "rz", "i"};
    while (qasmFile >> str) {
        string space_delimiter = " ";
        string type = str.substr(0, str.find(" "));
        type = str.substr(0, str.find("("));
        string isCX = type.substr(0, 2);
        
        // cout << type<<endl;
        
        if (isCX != "cx") {
            if (find(begin(singleList), end(singleList), type) != end(singleList)){
                qasmFile >> str;
                string singleQubitId = str.substr(2, str.size() - 4);
                
                unsigned q = stoul(singleQubitId);

                tuple<unsigned, unsigned> temp(q, q);
                topo::Gate tempGate(gateId, Operator::Single, temp);
                tempGate.set_prev(_lastGate[q], _lastGate[q]);

                if (_lastGate[q] != unsigned(-1))
                    _gates[_lastGate[q]].add_next(gateId);

                // update Id
                _lastGate[q] = gateId;
                _gates.push_back(move(tempGate));
                gateId++;
            }
            else{
                if (type != "creg" && type != "qreg"){
                    cerr << "Unseen Gate "<<type<<endl; 
                    assert(true);
                    exit(0); 
                }
                else qasmFile >> str;
            }
            // qasmFile >> str;
        }
        else {
            qasmFile >> str;
            // cout << str << endl;
            string delimiter = ",";
            string token = str.substr(0, str.find(delimiter));
            string qubitId = token.substr(2, token.size() - 3);
            unsigned q1 = stoul(qubitId);
            token = str.substr(str.find(delimiter) + 1,
                               str.size() - str.find(delimiter) - 2);
            qubitId = token.substr(2, token.size() - 3);
            unsigned q2 = stoul(qubitId);

            // if (lastCnotWith[q1].first == q2 && lastCnotWith[q2].first == q1) {
                // Assert when Three Consecutive CNOTs
                // assert(lastCnotWith[q1].second < 2 &&
                //        lastCnotWith[q2].second < 2);

                // lastCnotWith[q1].second++;
                // lastCnotWith[q2].second++;
            // } else {
                // lastCnotWith[q1].first = q2;
                // lastCnotWith[q2].first = q1;
                // lastCnotWith[q1].second = 1;
                // lastCnotWith[q2].second = 1;

                tuple<unsigned, unsigned> temp(q1, q2);
                topo::Gate tempGate(gateId, Operator::CX, temp);
                tempGate.set_prev(_lastGate[q1], _lastGate[q2]);

                if (_lastGate[q1] != unsigned(-1))
                    _gates[_lastGate[q1]].add_next(gateId);
                if (_lastGate[q1] != _lastGate[q2]) {
                    if (_lastGate[q2] != unsigned(-1))
                        _gates[_lastGate[q2]].add_next(gateId);
                }

                // update Id
                _lastGate[q1] = gateId;
                _lastGate[q2] = gateId;
                _gates.push_back(move(tempGate));
                gateId++;
            // }
        }
    }
    for (size_t i = 0; i < _gates.size(); i++) {
        if (_gates[i].is_avail())
            _avail_gates.push_back(i);
    }
#ifdef DEBUG
    // print_gates_with_next();
    // print_gates_with_prev();
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