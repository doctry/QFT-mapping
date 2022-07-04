#include <gsl/gsl_siman.h>
#include <thread>
#include "flow.hpp"

/* set up parameters for this simulated annealing run */

#define N_TRIES 200        /* how many points do we try before stepping */
#define ITERS_FIXED_T 2000 /* how many iterations for each T? */
#define STEP_SIZE 1.0      /* max step size in random walk */
#define K 1.0              /* Boltzmann constant */
#define T_INITIAL 5000.0   /* initial temperature */
#define MU_T 1.002         /* damping factor for temperature */
#define T_MIN 5.0e-1

gsl_siman_params_t params = {N_TRIES,   ITERS_FIXED_T, STEP_SIZE, K,
                             T_INITIAL, MU_T,          T_MIN};

/* now some functions to test in one dimension */
double E1(void* xp) {
    double x = *((double*)xp);

    return exp(-pow((x - 1.0), 2.0)) * sin(8 * x);
}

double M1(void* xp, void* yp) {
    double x = *((double*)xp);
    double y = *((double*)yp);

    return fabs(x - y);
}

void S1(const gsl_rng* r, void* xp, double step_size) {
    double old_x = *((double*)xp);
    double new_x;

    double u = gsl_rng_uniform(r);
    new_x = u * 2 * step_size - step_size + old_x;

    memcpy(xp, &new_x, sizeof(new_x));
}

void P1(void* xp) {
    printf("%12g", *((double*)xp));
}

void update_topo2device(
    std::tuple<std::vector<unsigned>, std::vector<unsigned>>& state) {
    auto& topo2device = std::get<0>(state);
    auto& device2topo = std::get<1>(state);
    for (unsigned i = 0; i < device2topo.size(); ++i) {
        unsigned topo = device2topo[i];
        if (topo == UINT_MAX) {
            continue;
        }
        topo2device[topo] = i;
    }
}

void sa_place(json& conf) {

}