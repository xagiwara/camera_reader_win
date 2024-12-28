#include "media-event.hpp"
#include "safe-release.hpp"

MediaEvent::MediaEvent(IMFMediaEvent *pEvent) : pEvent(pEvent) {
    pEvent->AddRef();
}

MediaEvent::~MediaEvent() {
    SafeRelease(&pEvent);
}

void MediaEvent::_register(pybind11::module m) {
    pybind11::class_<MediaEvent>(m, "MediaEvent")
    ;
}
