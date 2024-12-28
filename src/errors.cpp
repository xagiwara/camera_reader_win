#include "errors/errors.hpp"
#include "errors/closed-handle.hpp"
#include "errors/hresult.hpp"
#include <iostream>
#include <bitset>

void CheckHResult(HRESULT hr) {
    if (FAILED(hr)) {
        // TODO: error message
        std::wcerr << L"HRESULT Error: " << std::hex << hr << std::endl;

        throw HResultError(hr);
    }
}

HResultError::HResultError(HRESULT hr) : std::exception("HRESULT error"), hr(hr) {
}

void HResultError::_register(pybind11::module &m) {
    pybind11::class_<HResultError>(m, "HResultError")
        .def_readonly("hr", &HResultError::hr);
}

void ClosedHandleError::_register(pybind11::module &m) {
    pybind11::class_<ClosedHandleError>(m, "ClosedHandleError");
}

void register_errors(pybind11::module &m) {
    HResultError::_register(m);
    ClosedHandleError::_register(m);
}