#include "errors/errors.hpp"
#include "errors/closed-handle.hpp"
#include "errors/hresult.hpp"
#include <iostream>
#include <bitset>
#include <sstream>
#include <WinBase.h>
#include <system_error>

void CheckHResult(HRESULT hr) {
    if (FAILED(hr)) {
        throw HResultError(hr);
    }
}

std::string getErrorMessage(HRESULT hr) {
    auto rawMessage = std::system_category().message(hr);
    if (rawMessage.length() == 0) {
        return "Unknown error";
    };

    auto wideMessageLength = MultiByteToWideChar(CP_ACP, 0, rawMessage.c_str(), -1, NULL, 0);
    LPWSTR wideMessage = new WCHAR[wideMessageLength];
    MultiByteToWideChar(CP_ACP, 0, rawMessage.c_str(), -1, wideMessage, wideMessageLength);

    auto messageLength = WideCharToMultiByte(CP_ACP, 0, wideMessage, -1, NULL, 0, NULL, NULL);
    LPSTR message = new CHAR[messageLength];
    WideCharToMultiByte(CP_ACP, 0, wideMessage, -1, message, messageLength, NULL, NULL);

    std::string result(message);
    delete[] wideMessage;
    delete[] message;
    return result;
}

std::string create_error_message(HRESULT hr) {
    std::ostringstream ss;
    ss << "HRESULT Error: 0x" << std::hex << hr; // << std::endl;
    // ss << getErrorMessage(hr);
    return ss.str();
}

HResultError::HResultError(HRESULT hr) : std::runtime_error(create_error_message(hr)), hr(hr), hresult_message(getErrorMessage(hr)) {
}

void HResultError::_register(pybind11::module &m) {
    pybind11::register_exception<HResultError>(m, "HResultError");
}

void ClosedHandleError::_register(pybind11::module &m) {
    pybind11::register_exception<ClosedHandleError>(m, "ClosedHandleError");
}

void register_errors(pybind11::module &m) {
    HResultError::_register(m);
    ClosedHandleError::_register(m);
}
