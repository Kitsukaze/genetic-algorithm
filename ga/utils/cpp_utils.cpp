#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "maximum_flow.cpp"
#include "maximum_weighted_flow.cpp"
#include "tsp_solver.cpp"
#include "weighted_flows_with_demands.cpp"
#include "weighted_random.cpp"

namespace py = pybind11;

PYBIND11_MODULE(cpp_utils, m)
{
    m.def(
        "weighted_flows_with_demands", &weighted_flows_with_demands,
        py::kw_only(), py::arg("size"), py::arg("demands"), py::arg("capacities"), py::arg("neighbors"), py::arg("flow_weights"), py::arg("source"), py::arg("sink"),
        py::call_guard<py::gil_scoped_release>());
    m.def(
        "maximum_flow", &maximum_flow,
        py::kw_only(), py::arg("size"), py::arg("capacities"), py::arg("neighbors"), py::arg("source"), py::arg("sink"),
        py::call_guard<py::gil_scoped_release>());
    m.def(
        "maximum_weighted_flow", &maximum_weighted_flow,
        py::kw_only(), py::arg("size"), py::arg("capacities"), py::arg("neighbors"), py::arg("flow_weights"), py::arg("source"), py::arg("sink"),
        py::call_guard<py::gil_scoped_release>());
    m.def(
        "tsp_solver", &tsp_solver,
        py::arg("cities"), py::kw_only(), py::arg("first") = 0,
        py::call_guard<py::gil_scoped_release>());
    m.def(
        "weighted_random", &weighted_random,
        py::arg("weights"), py::kw_only(), py::arg("count") = 1,
        py::call_guard<py::gil_scoped_release>());
}
