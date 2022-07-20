#include <gsl/gsl_siman.h>
#include <thread>
#include "flow.hpp"

class SAData {
   public:
    SAData(json& conf, unsigned t, unsigned d)
        : _conf(conf), _topo_num(t), _device_num(d) {
        for (unsigned i = 0; i < _topo_num; ++i) {
            _device2topo.push_back(i);
            _topo2device.push_back(i);
        }
        for (unsigned i = _topo_num; i < _device_num; ++i) {
            _device2topo.push_back(UINT_MAX);
        }
    }

    void step(unsigned rand_num) {
        unsigned choose_topo = rand_num % _topo_num;
        unsigned choose_device = _topo2device[choose_topo];

        if (choose_device == _device_num - 1) {
            swap_q(choose_device, 0);
        } else {
            swap_q(choose_device, choose_device + 1);
        }
    }

    json& get_conf() { return _conf; }
    std::vector<unsigned>& get_topo2device() { return _topo2device; }

    unsigned get_distance(SAData& other) {
        unsigned ret = 0;
        assert(_device2topo.size() == other._device2topo.size());
        for (unsigned i = 0; i < _device2topo.size(); ++i) {
            ret += _device2topo[i] == other._device2topo[i] ? 0 : 1;
        }
        return ret;
    }

   private:
    json& _conf;
    unsigned _topo_num;
    unsigned _device_num;
    std::vector<unsigned> _device2topo;
    std::vector<unsigned> _topo2device;

    void swap_q(unsigned device0, unsigned device1) {
        std::swap(_device2topo[device0], _device2topo[device1]);

        assign_q(_device2topo[device0], device0);
        assign_q(_device2topo[device1], device1);
    }

    void assign_q(unsigned topo, unsigned device) {
        if (topo == UINT_MAX) {
            return;
        }
        assert(_topo2device[topo] != device);
        _topo2device[topo] = device;
    }
};

/* set up parameters for this simulated annealing run */

#define N_TRIES 2        /* how many points do we try before stepping */
#define ITERS_FIXED_T 20 /* how many iterations for each T? */
#define STEP_SIZE 1.0    /* max step size in random walk */
#define K 1.0            /* Boltzmann constant */
#define T_INITIAL 1000.0 /* initial temperature */
#define MU_T 1.2         /* damping factor for temperature */
#define T_MIN 5.0e-1

gsl_siman_params_t params = {N_TRIES,   ITERS_FIXED_T, STEP_SIZE, K,
                             T_INITIAL, MU_T,          T_MIN};

/* energy */
double E(void* xp) {
    SAData* sa_data = (SAData*)xp;
    return flow(sa_data->get_conf(), sa_data->get_topo2device(), false);
}

double M(void* xp, void* yp) {
    SAData* sa_data_x = (SAData*)xp;
    SAData* sa_data_y = (SAData*)yp;
    assert(sa_data_x->get_distance(*sa_data_y) ==
           sa_data_y->get_distance(*sa_data_x));
    return sa_data_x->get_distance(*sa_data_y);
}

/* take a step space */
void S(const gsl_rng* r, void* xp, double step_size) {
    (void)step_size;
    SAData* sa_data = (SAData*)xp;
    sa_data->step(gsl_rng_get(r));
}

void P(void* xp) {
    SAData* sa_data = (SAData*)xp;
    std::cout << sa_data->get_topo2device() << std::endl;
}

void sa_place(json& conf) {
    // init data
    unsigned t_num = topo_num(conf);
    unsigned d_num = device_num(conf);
    SAData sa_data(conf, t_num, d_num);

    const gsl_rng* r = gsl_rng_alloc(gsl_rng_env_setup());

    gsl_siman_solve(r, &sa_data, E, S, M, P, NULL, NULL, NULL, sizeof(sa_data),
                    params);

    printf("# best init placement:\n");
    std::cout << sa_data.get_topo2device() << std::endl;
    flow(conf, sa_data.get_topo2device(), true);
}
