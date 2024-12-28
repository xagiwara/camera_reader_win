#pragma once

#include <pybind11/pybind11.h>
#include <mfapi.h>
#include "guid.hpp"

const WCHAR *MFMediaTypeGUID2String(const GUID &guid);

class MediaType {
public:
    IMFMediaType *pMediaType = NULL;
    MediaType();
    MediaType(IMFMediaType *pMediaType);
    ~MediaType();

    Guid get_major_type();
    void set_major_type(Guid guid);

    Guid get_subtype();
    void set_subtype(Guid guid);

    pybind11::tuple get_frame_size();
    void set_frame_size(pybind11::tuple size);

    pybind11::tuple get_frame_rate();
    void set_frame_rate(pybind11::tuple rate);

    bool compressed_format();

    static void _register(pybind11::module &m);
};
