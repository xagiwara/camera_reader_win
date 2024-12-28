#pragma once

#include <pybind11/stl.h>
#include <mfidl.h>

class MediaEvent {
    IMFMediaEvent *pEvent;
public:
    MediaEvent(IMFMediaEvent *pEvent);
    ~MediaEvent();

    static void _register(pybind11::module m);
};
