#include <iostream>
#include <mfapi.h>
#include <mmdeviceapi.h>

#include "safe-release.hpp"
#include "media-source-activate.hpp"
#include "media-source.hpp"
#include "errors/hresult.hpp"
#include "guid.hpp"

void MediaSourceActivate::_register(pybind11::module &m) {
    pybind11::class_<MediaSourceActivate>(m, "MediaSourceActivate")
        .def_readonly("friendly_name", &MediaSourceActivate::friendly_name)
        .def_readonly("major_type", &MediaSourceActivate::major_type)
        .def_readonly("subtype", &MediaSourceActivate::subtype)
        .def_readonly("source_type", &MediaSourceActivate::source_type)
        .def_readonly("audio_endpoint_id", &MediaSourceActivate::audio_endpoint_id)
        .def_readonly("video_hardware", &MediaSourceActivate::video_hardware)
        .def_readonly("video_symbolic_link", &MediaSourceActivate::video_symbolic_link)
        .def("open", &MediaSourceActivate::open)
    ;

    m.def("enum_device_sources", &enum_device_sources, pybind11::arg("device_type"), pybind11::arg("audio_role") = pybind11::none());
}


MediaSourceActivate::~MediaSourceActivate() {
    if (friendly_name != NULL) CoTaskMemFree(friendly_name);
    if (major_type != NULL) delete major_type;
    if (subtype != NULL) delete subtype;
    if (audio_endpoint_id != NULL) CoTaskMemFree(audio_endpoint_id);
    if (video_hardware != NULL) delete video_hardware;
    if (video_symbolic_link != NULL) CoTaskMemFree(video_symbolic_link);
}

MediaSource *MediaSourceActivate::open() {
    if (std::wstring(source_type) == L"audio") {
        return new MediaSource(source_type, audio_endpoint_id);
    }

    if (std::wstring(source_type) == L"video") {
        return new MediaSource(source_type, video_symbolic_link);
    }

    return nullptr;
}

MediaSourceActivate::MediaSourceActivate(IMFActivate *pActivate) {
    UINT32 cchLength;

    CheckHResult(pActivate->GetAllocatedString(
        MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME,
        &friendly_name,
        &cchLength
    ));

    MFT_REGISTER_TYPE_INFO register_type_info;
    if (!pActivate->GetBlob(
        MF_DEVSOURCE_ATTRIBUTE_MEDIA_TYPE,
        (UINT8 *)&register_type_info,
        sizeof(register_type_info),
        NULL
    )) {
        this->major_type = new Guid(register_type_info.guidMajorType);
        this->subtype = new Guid(register_type_info.guidSubtype);
    }

    GUID source_type;
    if (!pActivate->GetGUID(
        MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
        &source_type
    )) {
        if (source_type == MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID) {
            this->source_type = L"video";
        } else if (source_type == MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_AUDCAP_GUID) {
            this->source_type = L"audio";
        }
    }

    pActivate->GetAllocatedString(
        MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_AUDCAP_ENDPOINT_ID,
        &audio_endpoint_id,
        &cchLength
    );

    UINT32 video_hardware;
    if (!pActivate->GetUINT32(
        MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_HW_SOURCE,
        &video_hardware
    )) {
        this->video_hardware = new bool;
        *this->video_hardware = !!video_hardware;
    }

    pActivate->GetAllocatedString(
        MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK,
        &video_symbolic_link,
        &cchLength
    );
}

std::vector<MediaSourceActivate*> enum_device_sources(std::wstring device_type, const WCHAR *audio_role) {
    IMFAttributes *pAttributes = NULL;
    std::vector<MediaSourceActivate*> devices;

    CheckHResult(MFCreateAttributes(&pAttributes, 1));
    try {
        if (device_type == L"video") {
            CheckHResult(pAttributes->SetGUID(
                MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
                MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID
            ));
        } else if (device_type == L"audio") {
            CheckHResult(pAttributes->SetGUID(
                MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
                MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_AUDCAP_GUID
            ));
            std::wstring audio_role_s(audio_role == NULL ? L"" : audio_role);
            if (audio_role_s == L"") {
            } else if (audio_role_s == L"console") {
                CheckHResult(pAttributes->SetUINT32(
                    MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_AUDCAP_ROLE,
                    eConsole
                ));
            } else if (audio_role_s == L"multimedia") {
                CheckHResult(pAttributes->SetUINT32(
                    MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_AUDCAP_ROLE,
                    eMultimedia
                ));
            } else if (audio_role_s == L"communications") {
                CheckHResult(pAttributes->SetUINT32(
                    MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_AUDCAP_ROLE,
                    eCommunications
                ));
            } else {
                throw std::invalid_argument("audio_role must be 'console', 'multimedia', or 'communications'");
            }
        } else {
            throw std::invalid_argument("device_type must be 'video' or 'audio'");
        }

        IMFActivate **ppDevices = NULL;
        UINT32 count;
        CheckHResult(MFEnumDeviceSources(pAttributes, &ppDevices, &count));
        try {
            for (int i = 0; i < count; i++) {
                auto device = ppDevices[i];
                devices.push_back(new MediaSourceActivate(device));
            }
        } catch (...) {
            for (int i = 0; i < count; i++) {
                SafeRelease(&ppDevices[i]);
            }
            CoTaskMemFree(ppDevices);
            throw;
        }
        for (int i = 0; i < count; i++) {
            SafeRelease(&ppDevices[i]);
        }
        CoTaskMemFree(ppDevices);

        SafeRelease(&pAttributes);

        return devices;
    } catch (...) {
        SafeRelease(&pAttributes);
        throw;
    }
}
