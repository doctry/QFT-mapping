#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include "device.h"
#include "qft_mapper.h"

PYBIND11_MODULE(duostra, m)
{
    m.doc() = "Dual-source Dijkstra algorithm"; // optional module docstring

    // qft_topo.h
    py::class_<topo::Gate>(m, "GateCpp")
        .def(py::init<std::tuple<unsigned, unsigned>>())
        .def("get_qubits", &topo::Gate::get_qubits);
    py::class_<topo::QFTTopology>(m, "QFTTopologyCpp").def(py::init<unsigned>());

    // device.h
    py::class_<Operator>(m, "OperatorCpp").def(py::init<>());
    py::class_<Operation>(m, "OperationCpp")
        .def(py::init<Operator, std::tuple<unsigned, unsigned>, std::tuple<unsigned, unsigned>>())
        .def("get_cost", &Operation::get_cost)
        .def("get_op_time", &Operation::get_op_time)
        .def("get_operator_name", &Operation::get_operator_name)
        .def("get_qubits", &Operation::get_qubits)
        .def("get_duration", &Operation::get_duration);
    py::class_<device::Device>(m, "DeviceCpp")
        .def(py::init<std::vector<std::vector<unsigned>> &, unsigned, unsigned>())
        .def("get_final_cost", &device::Device::get_final_cost)
        .def("get_swap_num", &device::Device::get_swap_num)
        .def("get_num_qubits", &device::Device::get_num_qubits)
        .def("get_operations", &device::Device::get_operations)
        .def("route", &device::Device::route)
        .def("compile_route", &device::Device::compile_route)
        .def("mapping", &device::Device::mapping);

    // qft_mapper.h
    py::class_<QFTPlacer>(m, "QFTPlacerCpp")
        .def(py::init<>())
        .def("place", &QFTPlacer::place);
    py::class_<QFTRouter>(m, "QFTRouterCpp")
        .def(py::init<device::Device &>())
        .def("get_gate_cost", &QFTRouter::get_gate_cost)
        .def("assign_gate", &QFTRouter::assign_gate);
    py::class_<QFTScheduler>(m, "QFTSchedulerCpp")
        .def(py::init<topo::QFTTopology &>())
        .def("assign_gates", &QFTScheduler::assign_gates);
}