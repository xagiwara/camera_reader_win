#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <mfidl.h>
#include "closable.hpp"

class MediaBuffer : public Closable {
protected:
    IMFMediaBuffer *pMediaBuffer;
public:
    MediaBuffer(IMFMediaBuffer *pMediaBuffer);

    void closer() override;
    DWORD get_current_length();
    void set_current_length(DWORD length);
    DWORD get_max_length();
    pybind11::bytes get_bytes();

    static void _register(pybind11::module m);
};

class MediaBuffer2D : public MediaBuffer {
    IMF2DBuffer *p2DBuffer;
public:
    MediaBuffer2D(IMFMediaBuffer *pMediaBuffer, IMF2DBuffer *p2DBuffer);

    pybind11::bytes get_bytes_2d();

    static void _register(pybind11::module m);
};

MediaBuffer *create_media_buffer(IMFMediaBuffer *pMediaBuffer);
