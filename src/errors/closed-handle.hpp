#pragma once

#include <pybind11/stl.h>
#include <mfidl.h>

class ClosedHandleError : public std::runtime_error {
public:
    ClosedHandleError() : std::runtime_error("The handle has been closed.") {}

    static void _register(pybind11::module &m);
};
