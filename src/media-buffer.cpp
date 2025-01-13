#include "media-buffer.hpp"
#include "errors/hresult.hpp"
#include "safe-release.hpp"

#pragma region MediaBuffer
MediaBuffer::MediaBuffer(IMFMediaBuffer *pMediaBuffer) : pMediaBuffer(pMediaBuffer) {
}

void MediaBuffer::closer() {
    SafeRelease(&pMediaBuffer);
}

DWORD MediaBuffer::get_current_length() {
    DWORD length;
    CheckHResult(pMediaBuffer->GetCurrentLength(&length));
    return length;
}

void MediaBuffer::set_current_length(DWORD length) {
    CheckHResult(pMediaBuffer->SetCurrentLength(length));
}

DWORD MediaBuffer::get_max_length() {
    DWORD length;
    CheckHResult(pMediaBuffer->GetMaxLength(&length));
    return length;
}

pybind11::bytes MediaBuffer::get_bytes() {
    DWORD length;
    BYTE *bytes;
    CheckHResult(pMediaBuffer->Lock(&bytes, NULL, &length));
    std::vector<uint8_t> result(bytes, bytes + length);
    CheckHResult(pMediaBuffer->Unlock());
    return pybind11::bytes(reinterpret_cast<const char *>(result.data()), result.size());
}

void MediaBuffer::_register(pybind11::module m) {
    pybind11::class_<MediaBuffer, Closable>(m, "MediaBuffer")
        .def_property("current_length", &MediaBuffer::get_current_length, &MediaBuffer::set_current_length)
        .def_property_readonly("max_length", &MediaBuffer::get_max_length)
        .def("get_bytes", &MediaBuffer::get_bytes)
    ;
}
#pragma endregion

#pragma region Buffer2D
MediaBuffer2D::MediaBuffer2D(IMFMediaBuffer *pMediaBuffer, IMF2DBuffer *p2DBuffer) : MediaBuffer(pMediaBuffer), p2DBuffer(p2DBuffer) {
}

pybind11::bytes MediaBuffer2D::get_bytes_2d() {
    DWORD size;
    CheckHResult(p2DBuffer->GetContiguousLength(&size));
    BYTE *bytes = new BYTE[size];
    CheckHResult(p2DBuffer->ContiguousCopyTo(bytes, size));
    pybind11::bytes result(reinterpret_cast<const char *>(bytes), size);
    delete[] bytes;
    return result;
}

void MediaBuffer2D::_register(pybind11::module m) {
    pybind11::class_<MediaBuffer2D, MediaBuffer>(m, "MediaBuffer2D")
        .def("get_bytes_2d", &MediaBuffer2D::get_bytes_2d)
    ;
}


#pragma endregion

MediaBuffer *create_media_buffer(IMFMediaBuffer *pMediaBuffer) {
    IMF2DBuffer *p2DBuffer = NULL;
    if (SUCCEEDED(pMediaBuffer->QueryInterface(__uuidof(IMF2DBuffer), (void **)&p2DBuffer))) {
        pMediaBuffer->Release();
        return new MediaBuffer2D(pMediaBuffer, p2DBuffer);
    }
    return new MediaBuffer(pMediaBuffer);
}
