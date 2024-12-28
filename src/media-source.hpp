#pragma once

#include <pybind11/stl.h>
#include <mfidl.h>

#include "closable.hpp"
#include "stream-descriptor.hpp"
#include "source-reader.hpp"

class MediaSource : public Closable {
private:
    IMFMediaSource *pMediaSource;
    IMFPresentationDescriptor *pPresentationDescriptor;

public:
    DWORD dwStreamDescriptorCount;
    MediaSource(std::wstring source_type, WCHAR *endpoint_id_or_symbolic_link);
    MediaSource(IMFMediaSource *pMediaSource);
    std::set<std::wstring> getCharacteristics();
    StreamDescriptorByIndex *__getitem__(int index);
    void closer() override;

    SourceReader *create_source_reader(
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
        void *fieldofuse_ulock_attribute);


    static void _register(pybind11::module m);
};
