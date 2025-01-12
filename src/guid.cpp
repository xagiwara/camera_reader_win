#include "guid.hpp"
#include "errors/hresult.hpp"

Guid::Guid() {
    ZeroMemory(&_data, sizeof(_data));
}

Guid::Guid(GUID guid) {
    _data = guid;
}

Guid::Guid(std::wstring str) {
    CheckHResult(CLSIDFromString(str.c_str(), &_data));
}

std::wstring Guid::__str__() {
    wchar_t buffer[39];
    CheckHResult(StringFromGUID2(_data, buffer, 39));
    return buffer;
}

bool Guid::__eq__(Guid other) {
    return IsEqualGUID(_data, other._data);
}

DWORD Guid::__hash__() const {
    unsigned long *longptr = (unsigned long *)&_data;
    return longptr[0] ^ longptr[1] ^ longptr[2] ^ longptr[3];
}

bool Guid::operator < (const Guid &other) const {
    return this->__hash__() < other.__hash__();
}

inline void def_guid(pybind11::class_<Guid> &clz, const CHAR name[], GUID guid) {
    clz.def_property_readonly_static(name, [guid](pybind11::object) {
        return Guid(guid);
    });
}

#define DEF_GUID(d, name) (def_guid(d, #name, name))

void Guid::_register(pybind11::module &m) {
    auto d = pybind11::class_<Guid>(m, "GUID")
        .def(pybind11::init<std::wstring>())
        .def("__str__", &Guid::__str__)
        .def("__hash__", &Guid::__hash__)
        .def("__eq__", &Guid::__eq__)
        .def("__neq__", [](Guid a, Guid b) { return !a.__eq__(b); });
    DEF_GUID(d, MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME);
}

