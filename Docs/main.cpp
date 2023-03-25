#include <iostream>
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>
#include <Insane/InsaneEmscripten.h>
#include <Insane/InsaneCryptography.h>
#include <random>

int main()
{
    USING_NS_INSANE_EMSCRIPTEN;
    USING_NS_INSANE_CRYPTO;
    Console::Log("Hello World!!! from InsaneEmscripten.");
    CryptoTests::Base64EncodingExtensionsTests();
}

EMSCRIPTEN_BINDINGS(exports)
{
    USING_NS_EMSCRIPTEN;
    USING_NS_INSANE_EMSCRIPTEN;
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
    function<val>("JsonSerialize", &Json::Serialize<>);
    function<val>("PromiseResolve", &Promise::Resolve<>);
    function<val>("PromiseReject", &Promise::Reject<>);
    function<val>("OperatorAdd", &Operator::Add);
    function<val>("OperatorSubtract", &Operator::Subtract);
    function<val>("OperatorMultiply", &Operator::Multiply);
    function<val>("OperatorDivide", &Operator::Divide);
    function<val>("OperatorImport", &Operator::ImportAsync);
    function<val>("JsLoadScriptAsync", &Js::LoadScriptAsync<>);
    function<>("JsGetPropertyName", &Js::GetPropertyName);
    EMSCRIPTEN_EXPORT_ALL_FUNCTORS(7);
    value_object<Browser::MimeType>("Mime")
        .field("Description", &Insane::Emscripten::Browser::MimeType::Description)
        .field("Description", &Insane::Emscripten::Browser::MimeType::Suffixes)
        .field("Type", &Insane::Emscripten::Browser::MimeType::Type);
    register_vector<Browser::MimeType>("VectorB");
    value_object<Browser::Plugin>("PluginClass").field("Name", &Browser::Plugin::Name).field("MimeTypes", &Browser::Plugin::MimeTypes);
    register_vector<Browser::Plugin>("VectorA");
}
