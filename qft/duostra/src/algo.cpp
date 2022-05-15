#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <tuple>
#include "algo.h"
using namespace std;

bool topo::Gate::is_avail()
{
    bool avail = true;
    for (size_t i = 0; i < _prevs.size(); i++)
    {
        if (!_prevs[i].second)
        {
            avail = false;
            break;
        }
    }
    return avail;
}

void topo::Gate::finished(unsigned prev)
{
#ifdef DEBUG
    bool check = false;
    for (size_t i = 0; i < _prevs.size(); i++)
    {
        if (_prevs[i].first == prev)
            check = true;
    }
    assert(check);
#endif
    for (size_t i = 0; i < _prevs.size(); i++)
    {
        if (_prevs[i].first == prev)
            _prevs[i].second = true;
    }
}

void topo::AlgoTopology::update_avail_gates(unsigned executed)
{
    assert(find(_avail_gates.begin(), _avail_gates.end(), executed) != _avail_gates.end());
    Gate &g_exec = _gates[executed];
    _avail_gates.erase(std::remove(_avail_gates.begin(), _avail_gates.end(), executed), _avail_gates.end());
    assert(g_exec.get_id() == executed);

    vector<unsigned> nexts = g_exec.get_nexts();

    for (unsigned i = 0; i < nexts.size(); i++)
    {
        unsigned n = nexts[i];
        _gates[n].finished(executed);
        if (_gates[n].is_avail())
            _avail_gates.push_back(n);
    }
}

void topo::AlgoTopology::parse(fstream &qasmFile)
{
    string str;
    for (int i = 0; i < 6; i++)
        qasmFile >> str;
    _num = stoi(str.substr(str.find("[") + 1, str.size() - str.find("[") - 3));
    for (size_t i = 0; i < _num; i++)
        _lastGate.push_back(-1);

    unsigned cnotId = 0;

    while (qasmFile >> str)
    {
        string type = str.substr(0, 2);

        if (type != "cx")
            qasmFile >> str;
        else
        {
            qasmFile >> str;
            // cout << str << endl;
            string delimiter = ",";
            string token = str.substr(0, str.find(delimiter));
            string qubitId = token.substr(2, token.size() - 3);
            unsigned q1 = stoul(qubitId);
            token = str.substr(str.find(delimiter) + 1, str.size() - str.find(delimiter) - 2);
            qubitId = token.substr(2, token.size() - 3);
            unsigned q2 = stoul(qubitId);
            tuple<unsigned, unsigned> temp(q1, q2);

            topo::Gate tempGate(cnotId, temp);
            tempGate.set_prev(_lastGate[q1], _lastGate[q2]);

            if (_lastGate[q1] != unsigned(-1))
                _gates[_lastGate[q1]].add_next(cnotId);
            if (_lastGate[q1] != _lastGate[q2])
            {
                if (_lastGate[q2] != unsigned(-1))
                    _gates[_lastGate[q2]].add_next(cnotId);
            }

            // update Id
            _lastGate[q1] = cnotId;
            _lastGate[q2] = cnotId;
            _gates.push_back(move(tempGate));
            cnotId++;
        }
    }
    for (size_t i = 0; i < _gates.size(); i++)
    {
        if (_gates[i].is_avail())
            _avail_gates.push_back(i);
    }
#ifdef DEBUG
    print_gates_with_next();
    print_gates_with_prev();
#endif
}

void topo::AlgoTopology::print_gates_with_next()
{
    cout << "Print successors of each gate" << endl;
    for (size_t i = 0; i < _gates.size(); i++)
    {
        vector<unsigned> temp = _gates[i].get_nexts();
        cout << _gates[i].get_id() << " || ";
        for (size_t j = 0; j < temp.size(); j++)
        {
            cout << temp[j] << " ";
        }
        cout << endl;
    }
}

void topo::AlgoTopology::print_gates_with_prev()
{
    cout << "Print predecessors of each gate" << endl;
    for (size_t i = 0; i < _gates.size(); i++)
    {
        vector<pair<unsigned, bool>> temp = _gates[i].get_prevs();
        cout << _gates[i].get_id() << " || ";
        for (size_t j = 0; j < temp.size(); j++)
        {
            cout << temp[j].first << "(" << temp[j].second << ") ";
        }
        cout << endl;
    }
}