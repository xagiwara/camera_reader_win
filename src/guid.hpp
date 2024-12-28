#pragma once

#include <pybind11/stl.h>
#include <mfidl.h>

class Guid {
public:
    GUID _data;
    Guid();
    Guid(GUID guid);
    Guid(std::wstring str);

    std::wstring __str__();
    bool __eq__(Guid other);
    DWORD __hash__() const;

    bool operator < (const Guid &other) const;

    static void _register(pybind11::module &m);
};
