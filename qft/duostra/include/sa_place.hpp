#include <gsl/gsl_siman.h>
#include <thread>
#include "flow.hpp"

class SAData {
   public:
    SAData(json& conf, size_t t, size_t d)
        : conf_(conf),
          topo_num_(t),
          dev_num_(d),
          dev_to_topo_({}),
          topo_to_dev_({}) {
        for (size_t i = 0; i < topo_num_; ++i) {
            dev_to_topo_.push_back(i);
            topo_to_dev_.push_back(i);
        }
        for (size_t i = topo_num_; i < dev_num_; ++i) {
            dev_to_topo_.push_back(size_t(-1));
        }
    }

    void step(size_t rand_num) {
        size_t choose_topo = rand_num % topo_num_;
        size_t choose_device = topo_to_dev_[choose_topo];

        if (choose_device == dev_num_ - 1) {
            swap_q(choose_device, 0);
        } else {
            swap_q(choose_device, choose_device + 1);
        }
    }

    json& get_conf() { return conf_; }
    std::vector<size_t>& get_topo2device() { return topo_to_dev_; }

    size_t get_distance(SAData& other) {
        size_t ret = 0;
        assert(dev_to_topo_.size() == other.dev_to_topo_.size());
        for (size_t i = 0; i < dev_to_topo_.size(); ++i) {
            ret += dev_to_topo_[i] == other.dev_to_topo_[i] ? 0 : 1;
        }
        return ret;
    }

   private:
    json& conf_;
    size_t topo_num_;
    size_t dev_num_;
    std::vector<size_t> dev_to_topo_;
    std::vector<size_t> topo_to_dev_;

    void swap_q(size_t device0, size_t device1) {
        std::swap(dev_to_topo_[device0], dev_to_topo_[device1]);

        assign_q(dev_to_topo_[device0], device0);
        assign_q(dev_to_topo_[device1], device1);
    }

    void assign_q(size_t topo, size_t device) {
        if (topo == size_t(-1)) {
            return;
        }
        assert(topo_to_dev_[topo] != device);
        topo_to_dev_[topo] = device;
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
void S(const gsl_rng* r, void* xp, [[maybe_unused]] double step_size) {
    SAData* sa_data = (SAData*)xp;
    sa_data->step(gsl_rng_get(r));
}

void P(void* xp) {
    SAData* sa_data = (SAData*)xp;
    std::cout << sa_data->get_topo2device() << std::endl;
}

void sa_place(json& conf) {
    // init data
    size_t t_num = topo_num(conf);
    size_t d_num = device_num(conf);
    SAData sa_data(conf, t_num, d_num);

    const gsl_rng* r = gsl_rng_alloc(gsl_rng_env_setup());

    gsl_siman_solve(r, &sa_data, E, S, M, P, NULL, NULL, NULL, sizeof(sa_data),
                    params);

    printf("# best init placement:\n");
    std::cout << sa_data.get_topo2device() << std::endl;
    flow(conf, sa_data.get_topo2device(), true);
}
