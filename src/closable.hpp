#pragma once

#include <pybind11/stl.h>
#include <mfidl.h>

class Closable {
public:
    virtual void closer() = 0;
    void close();
    bool closed = false;

    static void _register(pybind11::module &m);
};