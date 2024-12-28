#pragma once

#include <pybind11/stl.h>
#include <mfidl.h>

#include "guid.hpp"
#include "media-source.hpp"

class MediaSourceActivate {
public:
    WCHAR *friendly_name = NULL;
    Guid *major_type = NULL;
    Guid *subtype = NULL;
    WCHAR *source_type = NULL;
    WCHAR *audio_endpoint_id = NULL;
    bool *video_hardware = NULL;
    WCHAR *video_symbolic_link = NULL;

    MediaSource *open();

    MediaSourceActivate(IMFActivate *pActivate);
    ~MediaSourceActivate();


    static void _register(pybind11::module &m);
};

std::vector<MediaSourceActivate*> enum_device_sources(std::wstring device_type, const WCHAR * audio_role);
