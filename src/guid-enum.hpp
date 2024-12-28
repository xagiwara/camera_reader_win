#pragma once

#include <pybind11/pybind11.h>
#include <mfapi.h>
#include "guid.hpp"

class GuidEnum {
    std::map<std::wstring, Guid> map;
    std::map<Guid, std::wstring> reverse_map;
public:
    GuidEnum(std::map<std::wstring, Guid> map);

    const std::wstring *get_name(Guid guid);
    Guid *__getattr__ (std::wstring name);

    static void _register(pybind11::module &m);
};
