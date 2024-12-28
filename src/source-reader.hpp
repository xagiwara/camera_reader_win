#pragma once

#include <pybind11/stl.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include "closable.hpp"
#include "media-event.hpp"
#include "sample.hpp"
#include "media-type.hpp"

class SourceReaderCallbackNative;

class SourceReaderCallback {
public:
    SourceReaderCallbackNative *pNative;

    SourceReaderCallback();
    ~SourceReaderCallback();

    virtual void on_event(int stream_index, MediaEvent *pEvent) {}
    virtual void on_flush(int stream_index) {}
    virtual void on_read_sample(int hr_status, int stream_index, int stream_flags, int64_t timestamp, Sample *pSample) {}

    static void _register(pybind11::module m);
};

class SourceReaderCallbackNative: public IMFSourceReaderCallback {
private:
    ULONG refCount = 1;
    SourceReaderCallback *callback = NULL;
public:
    SourceReaderCallbackNative(SourceReaderCallback *callback);

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
    SourceReaderCallbackNative *callback = nullptr;
public:
    SourceReader(IMFSourceReader *pSourceReader, SourceReaderCallbackNative *callback = nullptr);
    void closer() override;

    MediaType *get_current_media_type(DWORD stream_index);
    void set_current_media_type(DWORD stream_index, MediaType *pMediaType);

    pybind11::tuple read_sample(DWORD stream_index, DWORD control_flags);
    void read_sample_async(DWORD stream_index, DWORD control_flags);

    static void _register(pybind11::module m);
};
