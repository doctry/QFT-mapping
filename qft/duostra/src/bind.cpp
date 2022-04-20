#include <pybind11/pybind11.h>

namespace py = pybind11;

#include "device.h"
#include "qft_mapper.h"

PYBIND11_MODULE(duostra, m) {
    m.doc() = "Dual-source Dijkstra algorithm"; // optional module docstring

    py::class_<>

    py::class_<QFTMapper>
}