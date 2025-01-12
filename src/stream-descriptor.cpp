#include <mfapi.h>
#include <guiddef.h>

#include "stream-descriptor.hpp"
#include "safe-release.hpp"
#include "errors/hresult.hpp"
#include "media-type.hpp"

StreamDescriptor::StreamDescriptor(IMFStreamDescriptor *pDescriptor) : pDescriptor(pDescriptor) {
    pDescriptor->GetMediaTypeHandler(&pMediaTypeHandler);
    CheckHResult(pMediaTypeHandler->GetMediaTypeCount(&__len__));
}

StreamDescriptor::~StreamDescriptor() {
    SafeRelease(&pDescriptor);
    SafeRelease(&pMediaTypeHandler);
}

Guid StreamDescriptor::major_type() {
    GUID guid;
    CheckHResult(pMediaTypeHandler->GetMajorType(&guid));
    return Guid(guid);
}

MediaType *StreamDescriptor::__getitem__(DWORD index) {
    IMFMediaType *pMediaType;
    CheckHResult(pMediaTypeHandler->GetMediaTypeByIndex(index, &pMediaType));
    auto result = new MediaType(pMediaType);
    SafeRelease(&pMediaType);
    return result;
}

StreamDescriptorByIndex::StreamDescriptorByIndex(IMFStreamDescriptor *pStreamDescriptor, IMFPresentationDescriptor *pPresentationDescriptor, DWORD index) : StreamDescriptor(pStreamDescriptor), pPresentationDescriptor(pPresentationDescriptor), index(index) {
    pPresentationDescriptor->AddRef();
}

StreamDescriptorByIndex::~StreamDescriptorByIndex() {
    SafeRelease(&pPresentationDescriptor);
}

void StreamDescriptor::_register(pybind11::module &m) {
    pybind11::class_<StreamDescriptor>(m, "StreamDescriptor")
        .def("__len__", [](StreamDescriptor &self) {return self.__len__; })
        .def("__getitem__", &StreamDescriptor::__getitem__);
}

void StreamDescriptorByIndex::set_selected(bool selected) {
    if (selected) {
        CheckHResult(pPresentationDescriptor->SelectStream(index));
    } else {
        CheckHResult(pPresentationDescriptor->DeselectStream(index));
    }
}

bool StreamDescriptorByIndex::get_selected() {
    BOOL selected;
    IMFStreamDescriptor *pDescriptor;
    CheckHResult(pPresentationDescriptor->GetStreamDescriptorByIndex(index, &selected, &pDescriptor));
    SafeRelease(&pDescriptor);
    return selected;
}

void StreamDescriptorByIndex::_register(pybind11::module &m) {
    pybind11::class_<StreamDescriptorByIndex, StreamDescriptor>(m, "StreamDescriptorByIndex")
        .def_property("selected", &StreamDescriptorByIndex::get_selected, &StreamDescriptorByIndex::set_selected)
        .def_property_readonly("major_type", &StreamDescriptor::major_type)
    ;
}
