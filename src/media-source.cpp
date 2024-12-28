#include "safe-release.hpp"
#include "media-source.hpp"
#include "errors/hresult.hpp"

MediaSource::MediaSource(std::wstring source_type, WCHAR *endpoint_id_or_symbolic_link) {
    IMFAttributes *pAttributes = NULL;

    CheckHResult(MFCreateAttributes(&pAttributes, 2));
    if (source_type == L"audio") {
        CheckHResult(pAttributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_AUDCAP_GUID));
        if (endpoint_id_or_symbolic_link) {
            CheckHResult(pAttributes->SetString(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_AUDCAP_ENDPOINT_ID, endpoint_id_or_symbolic_link));
        }
    } else if (source_type == L"video") {
        CheckHResult(pAttributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID));
        if (endpoint_id_or_symbolic_link) {
            CheckHResult(pAttributes->SetString(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK, endpoint_id_or_symbolic_link));
        }
    }
    IMFMediaSource *pMediaSource = NULL;
    CheckHResult(MFCreateDeviceSource(pAttributes, &pMediaSource));
    SafeRelease(&pAttributes);
    this->pMediaSource = pMediaSource;
    CheckHResult(pMediaSource->CreatePresentationDescriptor(&pPresentationDescriptor));
    CheckHResult(pPresentationDescriptor->GetStreamDescriptorCount(&dwStreamDescriptorCount));
}

MediaSource::MediaSource(IMFMediaSource *pMediaSource) {
    this->pMediaSource = pMediaSource;
    // TODO: fix leaking memory
    CheckHResult(pMediaSource->CreatePresentationDescriptor(&pPresentationDescriptor));
    CheckHResult(pPresentationDescriptor->GetStreamDescriptorCount(&dwStreamDescriptorCount));
}

void MediaSource::closer() {
    SafeRelease(&pPresentationDescriptor);
    if (pMediaSource != NULL) {
        pMediaSource->Shutdown();
        SafeRelease(&pMediaSource);
    }
}

std::set<std::wstring> MediaSource::getCharacteristics() {
    std::set<std::wstring> characteristics;
    DWORD dwCharacteristics = 0;
    CheckHResult(pMediaSource->GetCharacteristics(&dwCharacteristics));
    if (dwCharacteristics & MFMEDIASOURCE_IS_LIVE) {
        characteristics.insert(L"live");
    }
    if (dwCharacteristics & MFMEDIASOURCE_CAN_SEEK) {
        characteristics.insert(L"seekable");
    }
    if (dwCharacteristics & MFMEDIASOURCE_CAN_PAUSE) {
        characteristics.insert(L"pausable");
    }
    if (dwCharacteristics & MFMEDIASOURCE_HAS_SLOW_SEEK) {
        characteristics.insert(L"slow_seek");
    }
    if (dwCharacteristics & MFMEDIASOURCE_HAS_MULTIPLE_PRESENTATIONS) {
        characteristics.insert(L"multiple_presentation");
    }
    if (dwCharacteristics & MFMEDIASOURCE_CAN_SKIPFORWARD) {
        characteristics.insert(L"skip_forwardable");
    }
    if (dwCharacteristics & MFMEDIASOURCE_CAN_SKIPBACKWARD) {
        characteristics.insert(L"skip_backwardable");
    }
    if (dwCharacteristics & MFMEDIASOURCE_DOES_NOT_USE_NETWORK) {
        characteristics.insert(L"no_network");
    }
    return characteristics;
}

StreamDescriptorByIndex *MediaSource::__getitem__(int index) {
    BOOL selected = 0;
    IMFStreamDescriptor *pStreamDescriptor = NULL;
    CheckHResult(pPresentationDescriptor->GetStreamDescriptorByIndex(index, &selected, &pStreamDescriptor));
    return new StreamDescriptorByIndex(pStreamDescriptor, pPresentationDescriptor, index);
}

void MediaSource::_register(pybind11::module m) {
    pybind11::class_<MediaSource, Closable>(m, "MediaSource")
        .def(pybind11::init<std::wstring, WCHAR *>(), pybind11::arg("source_type"), pybind11::arg("endpoint_id_or_symbolic_link") = nullptr)
        .def_property_readonly("characteristics", &MediaSource::getCharacteristics)
        .def("__len__", [](MediaSource &self) { return self.dwStreamDescriptorCount; })
        .def("__getitem__", &MediaSource::__getitem__)
        .def("create_source_reader", &MediaSource::create_source_reader,
            pybind11::arg("low_latency") = nullptr,
            pybind11::arg("readwrite_disable_converters") = nullptr,
            pybind11::arg("readwrite_enable_hardware_transforms") = nullptr,
            pybind11::arg("source_reader_async_callback") = nullptr,
            pybind11::arg("source_reader_d3d_manager") = nullptr,
            pybind11::arg("source_reader_disable_dxva") = nullptr,
            pybind11::arg("source_reader_disconnect_mediasource_on_shutdown") = nullptr,
            pybind11::arg("source_reader_enable_advanced_video_processing") = nullptr,
            pybind11::arg("source_reader_enable_video_processing") = nullptr,
            pybind11::arg("source_reader_mediasource_config") = nullptr,
            pybind11::arg("source_reader_passthrough_mode") = nullptr,
            pybind11::arg("fieldofuse_ulock_attribute") = nullptr
        )
    ;
}
