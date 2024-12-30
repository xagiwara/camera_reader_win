#pragma once

#include <pybind11/stl.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include "closable.hpp"
#include "media-event.hpp"
#include "sample.hpp"
#include "media-type.hpp"

using SourceReaderOnEventCallback = std::function<void(int stream_index, MediaEvent *pEvent)>;
using SourceReaderOnFlushCallback = std::function<void(int stream_index)>;
using SourceReaderOnReadSampleCallback = std::function<void(int hr_status, int stream_index, int stream_flags, int64_t timestamp, Sample *pSample)>;

class SourceReaderCallbackNative;

class SourceReaderCallback {
public:
    SourceReaderCallbackNative *pNative;
    SourceReaderOnEventCallback on_event = nullptr;
    SourceReaderOnFlushCallback on_flush = nullptr;
    SourceReaderOnReadSampleCallback on_read_sample = nullptr;

    SourceReaderCallback();
    ~SourceReaderCallback();

    static void _register(pybind11::module m);
};

class SourceReaderCallbackNative: public IMFSourceReaderCallback {
private:
    ULONG refCount = 1;
    SourceReaderCallback *callback = NULL;
public:
    SourceReaderCallbackNative(SourceReaderCallback *callback);
    ~SourceReaderCallbackNative();

    HRESULT OnEvent(DWORD dwStreamIndex, IMFMediaEvent *pEvent) override;
    HRESULT OnFlush(DWORD dwStreamIndex) override;
    HRESULT OnReadSample(HRESULT hrStatus, DWORD dwStreamIndex, DWORD dwStreamFlags, LONGLONG llTimestamp, IMFSample *pSample) override;

    ULONG AddRef() override;
    ULONG Release() override;
    HRESULT QueryInterface(REFIID riid, void **ppvObject) override;
};

class SourceReader : public Closable {
private:
    IMFSourceReader *pSourceReader = nullptr;
    SourceReaderCallback *callback = nullptr;
public:
    SourceReader(IMFSourceReader *pSourceReader, SourceReaderCallback *callback = nullptr);
    void closer() override;

    MediaType *get_current_media_type(DWORD stream_index);
    void set_current_media_type(DWORD stream_index, MediaType *pMediaType);

    pybind11::tuple read_sample(DWORD stream_index, DWORD control_flags);
    void read_sample_async(DWORD stream_index, DWORD control_flags);

    static void _register(pybind11::module m);
};
