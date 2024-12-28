#include <pybind11/stl.h>
#include "media-type.hpp"
#include "safe-release.hpp"
#include "errors/hresult.hpp"
#include "guid.hpp"
#include "guid-enum.hpp"

MediaType::MediaType() {
    CheckHResult(MFCreateMediaType(&pMediaType));
}

MediaType::MediaType(IMFMediaType *pMediaType) : pMediaType(pMediaType) {
    pMediaType->AddRef();
}

MediaType::~MediaType() {
    SafeRelease(&pMediaType);
}

Guid MediaType::get_major_type() {
    GUID guid;
    CheckHResult(pMediaType->GetGUID(MF_MT_MAJOR_TYPE, &guid));
    return Guid(guid);
}

void MediaType::set_major_type(Guid guid) {
    CheckHResult(pMediaType->SetGUID(MF_MT_MAJOR_TYPE, guid._data));
}

Guid MediaType::get_subtype() {
    GUID guid;
    CheckHResult(pMediaType->GetGUID(MF_MT_SUBTYPE, &guid));
    return Guid(guid);
}

void MediaType::set_subtype(Guid guid) {
    CheckHResult(pMediaType->SetGUID(MF_MT_SUBTYPE, guid._data));
}

bool MediaType::compressed_format() {
    BOOL compressed;
    CheckHResult(pMediaType->IsCompressedFormat(&compressed));
    return compressed;
}

pybind11::tuple MediaType::get_frame_size() {
    UINT32 width, height;
    CheckHResult(MFGetAttributeSize(pMediaType, MF_MT_FRAME_SIZE, &width, &height));
    return pybind11::make_tuple(width, height);
}

void MediaType::set_frame_size(pybind11::tuple size) {
    CheckHResult(MFSetAttributeSize(pMediaType, MF_MT_FRAME_SIZE, size[0].cast<UINT32>(), size[1].cast<UINT32>()));
}

pybind11::tuple MediaType::get_frame_rate() {
    UINT32 numerator, denominator;
    CheckHResult(MFGetAttributeRatio(pMediaType, MF_MT_FRAME_RATE, &numerator, &denominator));
    return pybind11::make_tuple(numerator, denominator);
}

void MediaType::set_frame_rate(pybind11::tuple rate) {
    CheckHResult(MFSetAttributeRatio(pMediaType, MF_MT_FRAME_RATE, rate[0].cast<UINT32>(), rate[1].cast<UINT32>()));
}

GuidEnum *createMediaTypes() {
    return new GuidEnum({
        { L"MFMediaType_Audio", Guid(MFMediaType_Audio) },
        { L"MFMediaType_Binary", Guid(MFMediaType_Binary) },
        { L"MFMediaType_FileTransfer", Guid(MFMediaType_FileTransfer) },
        { L"MFMediaType_HTML", Guid(MFMediaType_HTML) },
        { L"MFMediaType_Image", Guid(MFMediaType_Image) },
        { L"MFMediaType_Metadata", Guid(MFMediaType_Metadata) },
        { L"MFMediaType_Protected", Guid(MFMediaType_Protected) },
        { L"MFMediaType_Perception", Guid(MFMediaType_Perception) },
        { L"MFMediaType_SAMI", Guid(MFMediaType_SAMI) },
        { L"MFMediaType_Script", Guid(MFMediaType_Script) },
        { L"MFMediaType_Video", Guid(MFMediaType_Video) },
        { L"MFMediaType_Stream", Guid(MFMediaType_Stream) },
    });
}

GuidEnum *createVideoFormats() {
    return new GuidEnum({
        { L"MFVideoFormat_RGB8", Guid(MFVideoFormat_RGB8) },
        { L"MFVideoFormat_RGB555", Guid(MFVideoFormat_RGB555) },
        { L"MFVideoFormat_RGB565", Guid(MFVideoFormat_RGB565) },
        { L"MFVideoFormat_RGB24", Guid(MFVideoFormat_RGB24) },
        { L"MFVideoFormat_RGB32", Guid(MFVideoFormat_RGB32) },
        { L"MFVideoFormat_ARGB32", Guid(MFVideoFormat_ARGB32) },
        { L"MFVideoFormat_A2R10G10B10", Guid(MFVideoFormat_A2R10G10B10) },
        { L"MFVideoFormat_A16B16G16R16F", Guid(MFVideoFormat_A16B16G16R16F) },
        { L"MFVideoFormat_AI44", Guid(MFVideoFormat_AI44) },
        { L"MFVideoFormat_AYUV", Guid(MFVideoFormat_AYUV) },
        { L"MFVideoFormat_I420", Guid(MFVideoFormat_I420) },
        { L"MFVideoFormat_IYUV", Guid(MFVideoFormat_IYUV) },
        { L"MFVideoFormat_NV11", Guid(MFVideoFormat_NV11) },
        { L"MFVideoFormat_NV12", Guid(MFVideoFormat_NV12) },
        { L"MFVideoFormat_NV21", Guid(MFVideoFormat_NV21) },
        { L"MFVideoFormat_UYVY", Guid(MFVideoFormat_UYVY) },
        { L"MFVideoFormat_Y41P", Guid(MFVideoFormat_Y41P) },
        { L"MFVideoFormat_Y41T", Guid(MFVideoFormat_Y41T) },
        { L"MFVideoFormat_Y42T", Guid(MFVideoFormat_Y42T) },
        { L"MFVideoFormat_YUY2", Guid(MFVideoFormat_YUY2) },
        { L"MFVideoFormat_YVU9", Guid(MFVideoFormat_YVU9) },
        { L"MFVideoFormat_YV12", Guid(MFVideoFormat_YV12) },
        { L"MFVideoFormat_YVYU", Guid(MFVideoFormat_YVYU) },
        { L"MFVideoFormat_P010", Guid(MFVideoFormat_P010) },
        { L"MFVideoFormat_P016", Guid(MFVideoFormat_P016) },
        { L"MFVideoFormat_P210", Guid(MFVideoFormat_P210) },
        { L"MFVideoFormat_P216", Guid(MFVideoFormat_P216) },
        { L"MFVideoFormat_v210", Guid(MFVideoFormat_v210) },
        { L"MFVideoFormat_v216", Guid(MFVideoFormat_v216) },
        { L"MFVideoFormat_v410", Guid(MFVideoFormat_v410) },
        { L"MFVideoFormat_Y210", Guid(MFVideoFormat_Y210) },
        { L"MFVideoFormat_Y216", Guid(MFVideoFormat_Y216) },
        { L"MFVideoFormat_Y410", Guid(MFVideoFormat_Y410) },
        { L"MFVideoFormat_Y416", Guid(MFVideoFormat_Y416) },
        { L"MFVideoFormat_L8", Guid(MFVideoFormat_L8) },
        { L"MFVideoFormat_L16", Guid(MFVideoFormat_L16) },
        { L"MFVideoFormat_D16", Guid(MFVideoFormat_D16) },
        { L"MFVideoFormat_DV25", Guid(MFVideoFormat_DV25) },
        { L"MFVideoFormat_DV50", Guid(MFVideoFormat_DV50) },
        { L"MFVideoFormat_DVC", Guid(MFVideoFormat_DVC) },
        { L"MFVideoFormat_DVH1", Guid(MFVideoFormat_DVH1) },
        { L"MFVideoFormat_DVHD", Guid(MFVideoFormat_DVHD) },
        { L"MFVideoFormat_DVSD", Guid(MFVideoFormat_DVSD) },
        { L"MFVideoFormat_DVSL", Guid(MFVideoFormat_DVSL) },
        { L"MFVideoFormat_H263", Guid(MFVideoFormat_H263) },
        { L"MFVideoFormat_H264", Guid(MFVideoFormat_H264) },
        { L"MFVideoFormat_H265", Guid(MFVideoFormat_H265) },
        { L"MFVideoFormat_H264_ES", Guid(MFVideoFormat_H264_ES) },
        { L"MFVideoFormat_HEVC", Guid(MFVideoFormat_HEVC) },
        { L"MFVideoFormat_HEVC_ES", Guid(MFVideoFormat_HEVC_ES) },
        { L"MFVideoFormat_M4S2", Guid(MFVideoFormat_M4S2) },
        { L"MFVideoFormat_MJPG", Guid(MFVideoFormat_MJPG) },
        { L"MFVideoFormat_MP43", Guid(MFVideoFormat_MP43) },
        { L"MFVideoFormat_MP4S", Guid(MFVideoFormat_MP4S) },
        { L"MFVideoFormat_MP4V", Guid(MFVideoFormat_MP4V) },
        { L"MFVideoFormat_MPEG2", Guid(MFVideoFormat_MPEG2) },
        { L"MFVideoFormat_VP80", Guid(MFVideoFormat_VP80) },
        { L"MFVideoFormat_VP90", Guid(MFVideoFormat_VP90) },
        { L"MFVideoFormat_MPG1", Guid(MFVideoFormat_MPG1) },
        { L"MFVideoFormat_MSS1", Guid(MFVideoFormat_MSS1) },
        { L"MFVideoFormat_MSS2", Guid(MFVideoFormat_MSS2) },
        { L"MFVideoFormat_WMV1", Guid(MFVideoFormat_WMV1) },
        { L"MFVideoFormat_WMV2", Guid(MFVideoFormat_WMV2) },
        { L"MFVideoFormat_WMV3", Guid(MFVideoFormat_WMV3) },
        { L"MFVideoFormat_WVC1", Guid(MFVideoFormat_WVC1) },
        { L"MFVideoFormat_420O", Guid(MFVideoFormat_420O) },
        { L"MFVideoFormat_AV1", Guid(MFVideoFormat_AV1) },
    });
}

void MediaType::_register(pybind11::module &m) {
    auto videoFormats = createVideoFormats();
    auto mediaTypes = createMediaTypes();
    pybind11::class_<MediaType>(m, "MediaType")
        .def(pybind11::init<>())
        .def_property("major_type", &MediaType::get_major_type, &MediaType::set_major_type)
        .def_property("subtype", &MediaType::get_subtype, &MediaType::set_subtype)
        .def_property_readonly("compressed_format", &MediaType::compressed_format)
        .def_property("frame_size", &MediaType::get_frame_size, &MediaType::set_frame_size)
        .def_property("frame_rate", &MediaType::get_frame_rate, &MediaType::set_frame_rate)
        .def_property_readonly_static("MAJOR_TYPES", [mediaTypes](pybind11::object) { return mediaTypes; })
        .def_property_readonly_static("VIDEO_FORMATS", [videoFormats](pybind11::object) { return videoFormats; })
    ;
}
