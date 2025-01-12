#pragma once

#include <pybind11/stl.h>
#include <mfidl.h>

class HResultError : public std::runtime_error {
public:
    HRESULT hr;
    std::string hresult_message;

    HResultError(HRESULT hr);

    static void _register(pybind11::module &m);
};

void CheckHResult(HRESULT hr);
