#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <mfapi.h>
#include <mfidl.h>
#include <iostream>

#include "media-source.hpp"
#include "media-source-activate.hpp"
#include "guid.hpp"
#include "errors/errors.hpp"
#include "stream-descriptor.hpp"
#include "closable.hpp"
#include "media-type.hpp"
#include "source-reader.hpp"
#include "media-event.hpp"
#include "guid-enum.hpp"
#include "sample.hpp"
#include "media-buffer.hpp"

#pragma comment(lib, "mf")
#pragma comment(lib, "mfreadwrite")
#pragma comment(lib, "mfplat")
#pragma comment(lib, "mfuuid")
#pragma comment(lib, "ole32")

PYBIND11_MODULE(_native, m) {
    MFStartup(MF_VERSION);

    m.doc() = "Camera Py Win Native Module";

    register_errors(m);
    Guid::_register(m);
    Closable::_register(m);

    MediaSource::_register(m);
    MediaSourceActivate::_register(m);
    StreamDescriptor::_register(m);
    StreamDescriptorByIndex::_register(m);
    MediaType::_register(m);
    SourceReader::_register(m);
    MediaEvent::_register(m);
    SourceReaderCallback::_register(m);
    GuidEnum::_register(m);
    Sample::_register(m);
    MediaBuffer::_register(m);
    MediaBuffer2D::_register(m);
}
