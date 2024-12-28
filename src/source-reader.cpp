#include <mfidl.h>
#include <mferror.h>
#include <mfreadwrite.h>
#include <iostream>

#include "source-reader.hpp"
#include "safe-release.hpp"
#include "media-source.hpp"
#include "errors/hresult.hpp"
#include "sample.hpp"

EXTERN_GUID(MF_SOURCE_READER_PASSTHROUGH_MODE, 0x043FF126, 0xFE2C, 0x4708, 0xa9, 0x6a, 0x85, 0x6a, 0x87, 0x93, 0x4e, 0x2e);

#pragma region SourceReader

SourceReader::SourceReader(IMFSourceReader *pSourceReader, SourceReaderCallbackNative *callback) : pSourceReader(pSourceReader), callback(callback) {
}

void SourceReader::closer() {
    SafeRelease(&pSourceReader);
}

void SourceReader::_register(pybind11::module m) {
    pybind11::class_<SourceReader, Closable>(m, "SourceReader")
        .def("get_current_media_type", &SourceReader::get_current_media_type)
        .def("set_current_media_type", &SourceReader::set_current_media_type)
        .def("read_sample", &SourceReader::read_sample, pybind11::arg("stream_index"), pybind11::arg("control_flags") = 0)
        .def("read_sample_async", &SourceReader::read_sample_async, pybind11::arg("stream_index"), pybind11::arg("control_flags") = 0)
    ;
}

SourceReader *MediaSource::create_source_reader(
    bool *low_latency,
    bool *readwrite_disable_converters,
    bool *readwrite_enable_hardware_transforms,
    SourceReaderCallback *source_reader_async_callback, // IMFSourceReaderCallback
    void *source_reader_d3d_manager, // MF_SOURCE_READER_D3D_MANAGER
    bool *source_reader_disable_dxva,
    bool *source_reader_disconnect_mediasource_on_shutdown,
    bool *source_reader_enable_advanced_video_processing,
    bool *source_reader_enable_video_processing,
    void *source_reader_mediasource_config,
    bool *source_reader_passthrough_mode,
    void *fieldofuse_ulock_attribute
) {
    IMFAttributes *pAttributes = NULL;
    SourceReaderCallbackNative *callback = nullptr;

    CheckHResult(MFCreateAttributes(&pAttributes, 1));
    if (low_latency != nullptr) {
        CheckHResult(pAttributes->SetUINT32(MF_LOW_LATENCY, *low_latency));
    }
    if (readwrite_disable_converters != nullptr) {
        CheckHResult(pAttributes->SetUINT32(MF_READWRITE_DISABLE_CONVERTERS, *readwrite_disable_converters));
    }
    if (readwrite_enable_hardware_transforms != nullptr) {
        CheckHResult(pAttributes->SetUINT32(MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS, *readwrite_enable_hardware_transforms));
    }
    if (source_reader_async_callback != nullptr) {
        CheckHResult(pAttributes->SetUnknown(MF_SOURCE_READER_ASYNC_CALLBACK, source_reader_async_callback->pNative));
    }
    if (source_reader_d3d_manager != nullptr) {
        // TODO: Implement this: CheckHResult(pAttributes->SetUnknown(MF_SOURCE_READER_D3D_MANAGER, source_reader_d3d_manager));
    }
    if (source_reader_disable_dxva != nullptr) {
        CheckHResult(pAttributes->SetUINT32(MF_SOURCE_READER_DISABLE_DXVA, *source_reader_disable_dxva));
    }
    if (source_reader_disconnect_mediasource_on_shutdown != nullptr) {
        CheckHResult(pAttributes->SetUINT32(MF_SOURCE_READER_DISCONNECT_MEDIASOURCE_ON_SHUTDOWN, *source_reader_disconnect_mediasource_on_shutdown));
    }
    if (source_reader_enable_advanced_video_processing != nullptr) {
        CheckHResult(pAttributes->SetUINT32(MF_SOURCE_READER_ENABLE_ADVANCED_VIDEO_PROCESSING, *source_reader_enable_advanced_video_processing));
    }
    if (source_reader_enable_video_processing != nullptr) {
        CheckHResult(pAttributes->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, *source_reader_enable_video_processing));
    }
    if (source_reader_mediasource_config != nullptr) {
        // TODO: Implement this: CheckHResult(pAttributes->SetUnknown(MF_SOURCE_READER_MEDIASOURCE_CONFIG, source_reader_mediasource_config));
    }
    if (source_reader_passthrough_mode != nullptr) {
        CheckHResult(pAttributes->SetUINT32(MF_SOURCE_READER_PASSTHROUGH_MODE, *source_reader_passthrough_mode));
    }
    if (fieldofuse_ulock_attribute != nullptr) {
        // TODO: Implement this: CheckHResult(pAttributes->SetUnknown(MFT_FIELDOFUSE_UNLOCK_Attribute, fieldofuse_ulock_attribute));
    }

    IMFSourceReader *pSourceReader;
    CheckHResult(MFCreateSourceReaderFromMediaSource(pMediaSource, pAttributes, &pSourceReader));
    SafeRelease(&pAttributes);
    return new SourceReader(pSourceReader, callback);
}

MediaType *SourceReader::get_current_media_type(DWORD stream_index) {
    IMFMediaType *pMediaType = NULL;
    CheckHResult(pSourceReader->GetCurrentMediaType(stream_index, &pMediaType));
    return new MediaType(pMediaType);
}

void SourceReader::set_current_media_type(DWORD stream_index, MediaType *pMediaType) {
    auto hr = pSourceReader->SetCurrentMediaType(stream_index, NULL, pMediaType->pMediaType);
    switch (hr) {
        case MF_E_INVALIDMEDIATYPE:
            throw std::exception("HRESULT Error: MF_E_INVALIDMEDIATYPE");
        case MF_E_INVALIDREQUEST:
            throw std::exception("HRESULT Error: MF_E_INVALIDREQUEST");
        case MF_E_INVALIDSTREAMNUMBER:
            throw std::exception("HRESULT Error: MF_E_INVALIDSTREAMNUMBER");
        case MF_E_TOPO_CODEC_NOT_FOUND:
            throw std::exception("HRESULT Error: MF_E_TOPO_CODEC_NOT_FOUND");
        default:
            CheckHResult(hr);
    }
}

pybind11::tuple SourceReader::read_sample(DWORD stream_index, DWORD control_flags) {
    IMFSample *pSample = NULL;
    DWORD streamIndex;
    DWORD stream_flags;
    LONGLONG timestamp;
    HRESULT hr = pSourceReader->ReadSample(stream_index, control_flags, &streamIndex, &stream_flags, &timestamp, &pSample);
    switch (hr) {
        case MF_E_INVALIDREQUEST:
            throw std::exception("HRESULT Error: MF_E_INVALIDREQUEST");
        case MF_E_INVALIDSTREAMNUMBER:
            throw std::exception("HRESULT Error: MF_E_INVALIDSTREAMNUMBER");
        case MF_E_NOTACCEPTING:
            throw std::exception("HRESULT Error: MF_E_NOTACCEPTING");
        case E_INVALIDARG:
            throw std::exception("HRESULT Error: E_INVALIDARG");
        default:
            CheckHResult(hr);
    }
    return pybind11::make_tuple(streamIndex, stream_flags, timestamp, pSample ? new Sample(pSample) : nullptr);
}

void SourceReader::read_sample_async(DWORD stream_index, DWORD control_flags) {
    auto hr = pSourceReader->ReadSample(stream_index, control_flags, NULL, NULL, NULL, NULL);
    switch (hr) {
        case MF_E_INVALIDREQUEST:
            throw std::exception("HRESULT Error: MF_E_INVALIDREQUEST");
        case MF_E_INVALIDSTREAMNUMBER:
            throw std::exception("HRESULT Error: MF_E_INVALIDSTREAMNUMBER");
        case MF_E_NOTACCEPTING:
            throw std::exception("HRESULT Error: MF_E_NOTACCEPTING");
        case E_INVALIDARG:
            throw std::exception("HRESULT Error: E_INVALIDARG");
        default:
            CheckHResult(hr);
    }
}
#pragma endregion


#pragma region SourceReaderCallback
void SourceReaderCallback::_register(pybind11::module m) {
    pybind11::class_<SourceReaderCallback>(m, "SourceReaderCallback")
        .def(pybind11::init<>())
        .def("on_event", &SourceReaderCallback::on_event)
        .def("on_flush", &SourceReaderCallback::on_flush)
        .def("on_read_sample", &SourceReaderCallback::on_read_sample)
    ;
}

SourceReaderCallback::SourceReaderCallback() {
    pNative = new SourceReaderCallbackNative(this);
}

SourceReaderCallback::~SourceReaderCallback() {
    SafeRelease(&pNative);
}
#pragma endregion


#pragma region SourceReaderCallbackNative
SourceReaderCallbackNative::SourceReaderCallbackNative(SourceReaderCallback *callback) : callback(callback) {
}

HRESULT SourceReaderCallbackNative::OnEvent(DWORD dwStreamIndex, IMFMediaEvent *pEvent) {
    std::cerr << "OnEvent" << std::endl;
    callback->on_event(dwStreamIndex, new MediaEvent(pEvent));
    return S_OK;
}

HRESULT SourceReaderCallbackNative::OnFlush(DWORD dwStreamIndex) {
    std::cerr << "OnFlush" << std::endl;
    callback->on_flush(dwStreamIndex);
    return S_OK;
}

HRESULT SourceReaderCallbackNative::OnReadSample(HRESULT hrStatus, DWORD dwStreamIndex, DWORD dwStreamFlags, LONGLONG llTimestamp, IMFSample *pSample) {
    std::cerr << "OnReadSample" << std::endl;
    callback->on_read_sample(hrStatus, dwStreamIndex, dwStreamFlags, llTimestamp, new Sample(pSample));
    return S_OK;
}

ULONG SourceReaderCallbackNative::AddRef() {
    return InterlockedIncrement(&refCount);
}

ULONG SourceReaderCallbackNative::Release() {
    ULONG refCount = InterlockedDecrement(&refCount);
    if (refCount == 0) {
        delete this;
    }
    return refCount;
}

HRESULT SourceReaderCallbackNative::QueryInterface(REFIID riid, void **ppvObject) {
    if (riid == __uuidof(IUnknown) || riid == __uuidof(IMFSourceReaderCallback)) {
        *ppvObject = static_cast<IMFSourceReaderCallback *>(this);
        AddRef();
        return S_OK;
    }
    *ppvObject = NULL;
    return E_NOINTERFACE;
}
#pragma endregion
