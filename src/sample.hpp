#pragma once

#include <pybind11/stl.h>
#include <mfidl.h>
#include "closable.hpp"
#include "media-buffer.hpp"

class Sample : public Closable {
private:
    IMFSample *pSample;
public:
    Sample(IMFSample *pSample);
    void closer();
    LONGLONG get_duration();
    LONGLONG get_sample_time();
    DWORD total_length();
    DWORD buffer_count();
    void set_duration(LONGLONG duration);
    void set_sample_time(LONGLONG sample_time);
    MediaBuffer *get_buffer_by_index(int index);
    MediaBuffer *convert_to_contiguous_buffer();

    static void _register(pybind11::module m);
};
