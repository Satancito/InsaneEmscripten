#include <iostream>
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>
#include <Insane/InsaneEmscripten.h>
#include <random>




int main()
{
    USING_INSANE_EMSCRIPTEN;
    Console::Log("Hello World!!! from InsaneEmscripten.");
   
}


EMSCRIPTEN_BINDINGS(exports)
{
    USING_EMSCRIPTEN;
    USING_INSANE_EMSCRIPTEN;
    function<val>(u8"BrowserGetNameAsync", &Browser::GetNameAsync<>);
    function<val>(u8"BrowserGetFingerprintAsync", &Browser::GetFingerprintAsync<>);
    function<val>(u8"BrowserGetOS", &Browser::GetOS<>);
    function<val>(u8"BrowserGetWebGLRenderer", &Browser::GetWebGLRenderer);
    function<val>(u8"BrowserGetWebGLVendor", &Browser::GetWebGLVendor);
    function<val>(u8"BrowserGetAvailableScreenHeight", &Browser::GetAvailableScreenHeight);
    function<val>(u8"BrowserGetAvailableScreenWidth", &Browser::GetAvailableScreenWidth);
    function<val>(u8"BrowserGetScreenHeight", &Browser::GetScreenHeight);
    function<val>(u8"BrowserGetScreenWidth", &Browser::GetScreenWidth);
    function<val>(u8"BrowserGetViewportHeight", &Browser::GetViewportHeight);
    function<val>(u8"BrowserGetViewportWidth", &Browser::GetViewportWidth);
    function<val>(u8"BrowserGetAvailableScreenOrientation", &Browser::GetAvailableScreenOrientation);
    function<val>(u8"BrowserGetScreenOrientation", &Browser::GetScreenOrientation);
    function<val>(u8"BrowserGetViewportOrientation", &Browser::GetViewportOrientation);
    function<val>(u8"BrowserGetAvailableScreenSize", &Browser::GetAvailableScreenSize);
    function<val>(u8"BrowserGetScreenSize", &Browser::GetScreenSize);
    function<val>(u8"BrowserGetViewportSize", &Browser::GetViewportSize);
    function<val>(u8"BrowserGetLanguages", &Browser::GetLanguages);
    function<val>(u8"BrowserGetLanguage", &Browser::GetLanguage);
    function<val>(u8"BrowserGetUserAgent", &Browser::GetUserAgent);
    function<val>(u8"BrowserGetTimezoneOffsetMinutes", &Browser::GetTimezoneOffsetMinutes);
    function<val>(u8"BrowserGetTimezoneOffsetSeconds", &Browser::GetTimezoneOffsetSeconds);
    function<val>(u8"BrowserGetTimezoneOffsetMilliseconds", &Browser::GetTimezoneOffsetMilliseconds);
    function<val>(u8"BrowserGetMaxTouchPoints", &Browser::GetMaxTouchPoints);
    function<val>(u8"BrowserGetDeviceMemory", &Browser::GetDeviceMemory);
    function<val>(u8"BrowserGetHardwareConcurrency", &Browser::GetHardwareConcurrency);
    function<val>(u8"BrowserGetPlugins", &Browser::GetPlugins);
    function<val>(u8"BrowserGetDoNotTrack", &Browser::GetDoNotTrack);
    function<val>(u8"BrowserHasCookiesSupport", &Browser::HasCookiesSupport);
    function<val>(u8"BrowserGetMimeTypes", &Browser::GetMimeTypes);
    function<val>(u8"JsonSerialize", &Json::Serialize<>);
    function<val>(u8"PromiseResolve", &Promise::Resolve<>);
    function<val>(u8"PromiseReject", &Promise::Reject<>);
    function<val>(u8"OperatorAdd", &Operator::Add);
    function<val>(u8"OperatorSubtract", &Operator::Subtract);
    function<val>(u8"OperatorMultiply", &Operator::Multiply);
    function<val>(u8"OperatorDivide", &Operator::Divide);
    function<val>(u8"OperatorImport", &Operator::ImportAsync);
    function<val>(u8"JsLoadScriptAsync", &Js::LoadScriptAsync<>);
    function<>(u8"JsGetPropertyName", &Js::GetPropertyName);
    EMSCRIPTEN_EXPORT_ALL_FUNCTORS(7);
    value_object<Browser::MimeType>(u8"Mime")
        .field("Description", &Insane::Emscripten::Browser::MimeType::Description)
        .field("Description", &Insane::Emscripten::Browser::MimeType::Suffixes)
        .field("Type", &Insane::Emscripten::Browser::MimeType::Type);
    register_vector<Browser::MimeType>("VectorB");
    value_object<Browser::Plugin>(u8"PluginClass").field("Name", &Browser::Plugin::Name).field("MimeTypes", &Browser::Plugin::MimeTypes);
    register_vector<Browser::Plugin>("VectorA");

    
}