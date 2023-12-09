#include <iostream>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <emscripten/emscripten.h>
#include <Insane/InsaneEmscripten.h>
#include <Insane/InsaneCryptography.h>
#include <Insane/InsaneString.h>
#include <Insane/InsanePreprocessor.h>
#include <Insane/InsaneTest.h>
#include <random>
#include <stdlib.h>
USING_NS_INSANE_EMSCRIPTEN;
USING_NS_INSANE_CRYPTO;
USING_NS_INSANE_STR;
USING_NS_INSANE_TEST;
USING_NS_EMSCRIPTEN;

#include <chrono>
#include <thread>

#ifndef LIB_COMPILE_TIME
#define LIB_PRODUCT_NAME "<No product>"
#define LIB_PRODUCT_VERSION "<No Version>"
#endif

using JsConsole = InsaneIO::Insane::Emscripten::Console;
using Terminal = InsaneIO::Insane::Core::Console;

int main()
{
    DebugExtensions::Debug(true);
    JsConsole::Log("Hello World!!! from . \""s + LIB_PRODUCT_NAME + " " + LIB_PRODUCT_VERSION + "\".");
    return EXIT_SUCCESS;
}

EMSCRIPTEN_BINDINGS(exports)
{
    EMSCRIPTEN_EXPORT_ALL_FUNCTORS(5, Insane);

    function<val>("BrowserGetNameAsync", &Browser::GetNameAsync<>);
    function<val>("BrowserGetFingerprintAsync", &Browser::GetFingerprintAsync<>);
    function<val>("BrowserGetOS", &Browser::GetOS<>);
    function<val>("BrowserGetWebGLRenderer", &Browser::GetWebGLRenderer);
    function<val>("BrowserGetWebGLVendor", &Browser::GetWebGLVendor);
    function<val>("BrowserGetAvailableScreenHeight", &Browser::GetAvailableScreenHeight);
    function<val>("BrowserGetAvailableScreenWidth", &Browser::GetAvailableScreenWidth);
    function<val>("BrowserGetScreenHeight", &Browser::GetScreenHeight);
    function<val>("BrowserGetScreenWidth", &Browser::GetScreenWidth);
    function<val>("BrowserGetViewportHeight", &Browser::GetViewportHeight);
    function<val>("BrowserGetViewportWidth", &Browser::GetViewportWidth);
    function<val>("BrowserGetAvailableScreenOrientation", &Browser::GetAvailableScreenOrientation);
    function<val>("BrowserGetScreenOrientation", &Browser::GetScreenOrientation);
    function<val>("BrowserGetViewportOrientation", &Browser::GetViewportOrientation);
    function<val>("BrowserGetAvailableScreenSize", &Browser::GetAvailableScreenSize);
    function<val>("BrowserGetScreenSize", &Browser::GetScreenSize);
    function<val>("BrowserGetViewportSize", &Browser::GetViewportSize);
    function<val>("BrowserGetLanguages", &Browser::GetLanguages);
    function<val>("BrowserGetLanguage", &Browser::GetLanguage);
    function<val>("BrowserGetUserAgent", &Browser::GetUserAgent);
    function<val>("BrowserGetTimezoneOffsetMinutes", &Browser::GetTimezoneOffsetMinutes);
    function<val>("BrowserGetTimezoneOffsetSeconds", &Browser::GetTimezoneOffsetSeconds);
    function<val>("BrowserGetTimezoneOffsetMilliseconds", &Browser::GetTimezoneOffsetMilliseconds);
    function<val>("BrowserGetMaxTouchPoints", &Browser::GetMaxTouchPoints);
    function<val>("BrowserGetDeviceMemory", &Browser::GetDeviceMemory);
    function<val>("BrowserGetHardwareConcurrency", &Browser::GetHardwareConcurrency);
    function<val>("BrowserGetPlugins", &Browser::GetPlugins);
    function<val>("BrowserGetDoNotTrack", &Browser::GetDoNotTrack);
    function<val>("BrowserHasCookiesSupport", &Browser::HasCookiesSupport);
    function<val>("BrowserGetMimeTypes", &Browser::GetMimeTypes);

    value_object<Browser::MimeType>("MimeType")
        .field("Description", &Browser::MimeType::Description)
        .field("Description", &Browser::MimeType::Suffixes)
        .field("Type", &Browser::MimeType::Type);
    register_vector<Browser::MimeType>("StdVectorMimeType");

    value_object<Browser::Plugin>("Plugin")
        .field("Name", &Browser::Plugin::Name)
        .field("MimeTypes", &Browser::Plugin::MimeTypes);
    register_vector<Browser::Plugin>("StdVectorBrowserPlugin");
}
