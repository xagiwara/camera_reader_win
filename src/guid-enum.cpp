#include "guid-enum.hpp"

std::map<Guid, std::wstring> flip_guid_map(std::map<std::wstring, Guid> &src) {
    std::map<Guid, std::wstring> dst;
    for (auto const &pair : src) {
        dst[pair.second] = pair.first;
    }
    return dst;
}

GuidEnum::GuidEnum(std::map<std::wstring, Guid> map) : map(map), reverse_map(flip_guid_map(map)) {
}

const std::wstring *GuidEnum::get_name(Guid guid) {
    if (reverse_map.find(guid) == reverse_map.end()) {
        return nullptr;
    }
    return &reverse_map[guid];
}

Guid *GuidEnum::__getattr__(std::wstring name) {
    if (map.find(name) == map.end()) {
        return nullptr;
    }
    return new Guid(map[name]);
}

void GuidEnum::_register(pybind11::module &m) {
    auto d = pybind11::class_<GuidEnum>(m, "GuidEnum")
        .def("__getattr__", &GuidEnum::__getattr__)
        .def("get_name", &GuidEnum::get_name);
}
