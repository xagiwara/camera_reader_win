#include "sample.hpp"
#include "safe-release.hpp"
#include "errors/hresult.hpp"

Sample::Sample(IMFSample *pSample) : pSample(pSample) {
}

void Sample::closer() {
    SafeRelease(&pSample);
}

LONGLONG Sample::get_duration() {
    LONGLONG duration = 0;
    CheckHResult(pSample->GetSampleDuration(&duration));
    return duration;
}

LONGLONG Sample::get_sample_time() {
    LONGLONG sample_time = 0;
    CheckHResult(pSample->GetSampleTime(&sample_time));
    return sample_time;
}

DWORD Sample::total_length() {
    DWORD total_length = 0;
    CheckHResult(pSample->GetTotalLength(&total_length));
    return total_length;
}

DWORD Sample::buffer_count() {
    DWORD buffer_count = 0;
    CheckHResult(pSample->GetBufferCount(&buffer_count));
    return buffer_count;
}

void Sample::set_duration(LONGLONG duration) {
    CheckHResult(pSample->SetSampleDuration(duration));
}

void Sample::set_sample_time(LONGLONG sample_time) {
    CheckHResult(pSample->SetSampleTime(sample_time));
}

MediaBuffer *Sample::get_buffer_by_index(int index) {
    IMFMediaBuffer *pMediaBuffer = NULL;
    CheckHResult(pSample->GetBufferByIndex(index, &pMediaBuffer));
    return create_media_buffer(pMediaBuffer);
}

MediaBuffer *Sample::convert_to_contiguous_buffer() {
    IMFMediaBuffer *pMediaBuffer = NULL;
    CheckHResult(pSample->ConvertToContiguousBuffer(&pMediaBuffer));
    return create_media_buffer(pMediaBuffer);
}

void Sample::_register(pybind11::module m) {
    pybind11::class_<Sample, Closable>(m, "Sample")
        .def_property("duration", &Sample::get_duration, &Sample::set_duration)
        .def_property("sample_time", &Sample::get_sample_time, &Sample::set_sample_time)
        .def_property_readonly("total_length", &Sample::total_length)
        .def_property_readonly("buffer_count", &Sample::buffer_count)
        .def("get_buffer_by_index", &Sample::get_buffer_by_index)
        .def("convert_to_contiguous_buffer", &Sample::convert_to_contiguous_buffer)
        .def("__len__", &Sample::buffer_count)

    ;
}
