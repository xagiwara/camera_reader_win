#pragma once

#include <pybind11/stl.h>
#include <mfidl.h>

#include "media-type.hpp"
#include "guid.hpp"

class StreamDescriptor {
private:
    IMFMediaTypeHandler *pMediaTypeHandler = NULL;
protected:
    IMFStreamDescriptor *pDescriptor = NULL;
public:
    StreamDescriptor(IMFStreamDescriptor *pDescriptor);
    ~StreamDescriptor();

    Guid major_type();
    DWORD __len__ = 0;
    MediaType *__getitem__(DWORD index);

    static void _register(pybind11::module &m);
};

class StreamDescriptorByIndex : public StreamDescriptor {
private:
    IMFPresentationDescriptor *pPresentationDescriptor = NULL;
    DWORD index = 0;
public:
    StreamDescriptorByIndex(IMFStreamDescriptor *pStreamDescriptor, IMFPresentationDescriptor *pPresentationDescriptor, DWORD index);
    ~StreamDescriptorByIndex();

    void set_selected(bool selected);
    bool get_selected();

    static void _register(pybind11::module &m);
};
