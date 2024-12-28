#include "closable.hpp"

void Closable::_register(pybind11::module &m) {
    pybind11::class_<Closable>(m, "Closable")
        .def_readonly("closed", &Closable::closed)
        .def("__enter__", [](Closable &self) { return &self; })
        .def("__exit__", [](Closable &self, pybind11::args &args) { self.close(); })
        .def("close", &Closable::close)
        .def("__del__", &Closable::close)
    ;
}

void Closable::close() {
    if (!closed) {
        closed = true;
        closer();
    }
}

