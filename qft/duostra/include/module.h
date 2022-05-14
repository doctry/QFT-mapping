#ifndef MODULE_H
#define MODULE_H

#include <vector>
#include <string>
#include <tuple>
#include <algorithm>
#include <assert.h>

class Gate
{
    public:
        Gate(unsigned id, std::tuple<unsigned, unsigned> qs) : _id(id), _qubits(qs){
            _prevs.clear();
            _nexts.clear();
        }
        Gate(const Gate &other) = delete;
        Gate(Gate &&other) : _id(other._id), _qubits(other._qubits), _prevs(std::move(other._prevs)),_nexts(std::move(other._nexts)){}

        unsigned get_id() const {   return _id; }

        void set_prev(unsigned a, unsigned b){
            std::pair<unsigned, bool> first(a,false);
            std::pair<unsigned, bool> second(b,false);
            if(a!=unsigned(-1)) _prevs.push_back(first);
            if (a!=b){
                if(b!=unsigned(-1)) _prevs.push_back(second);
            }
        }
        
        void add_next(unsigned n){  _nexts.push_back(n); }
        void finished(unsigned);
        bool is_avail();

        const std::vector<std::pair<unsigned, bool>>& get_prevs() const {   return _prevs;  }
        const std::vector<unsigned>& get_nexts()             const {   return _nexts;  }

    private:
        unsigned _id;
        std::tuple<unsigned, unsigned> _qubits;
        std::vector<std::pair<unsigned, bool>> _prevs;
        std::vector<unsigned> _nexts;
};

class Topology
{
    public:
        Topology(unsigned num) : _num(num){
            assert(num > 0);
            for(size_t i=0;i<num;i++)   _lastGate.push_back(-1);
        }
        Topology(const Topology &other) = delete;
        Topology(Topology &&other) : _gates(std::move(other._gates)), _avail_gates(std::move(other._avail_gates)) {}
        
        void parse(std::fstream&);

        const unsigned get_num_qubits() const   { return _num;          }
        const unsigned get_num_gates()  const   { return _gates.size(); }
        Gate &get_gate(const unsigned i)        { return _gates[i];     }
        
        std::vector<unsigned>& get_avail_gates()     { return _avail_gates;  }

        void update_avail_gates(unsigned);
        void print_gates_with_next();
        void print_gates_with_prev();

    private:
        unsigned _num;
        std::vector<Gate> _gates;
        std::vector<unsigned> _avail_gates;
        std::vector<unsigned> _lastGate;
};
#endif  // MODULE_H