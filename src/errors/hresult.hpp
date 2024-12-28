#pragma once

#include <pybind11/stl.h>
#include <mfidl.h>

class HResultError : public std::exception {
public:
    HRESULT hr;

    HResultError(HRESULT hr);

    static void _register(pybind11::module &m);
};

void CheckHResult(HRESULT hr);