#include <iostream>
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>
#include <Insane/InsaneEmscripten.h>
#include <Insane/InsaneCryptography.h>
#include <Insane/InsaneString.h>
#include <Insane/InsaneTest.h>
#include <random>

int main()
{
    USING_NS_INSANE_EMSCRIPTEN;
    USING_NS_INSANE_CRYPTO;
    USING_NS_INSANE_STR;
    USING_NS_INSANE_TEST;
    Console::Log("Hello World!!! from InsaneEmscripten.");
    CryptoTests::Base32EncodingExtensionsTests();
    CryptoTests::Base64EncodingExtensionsTests();
    TestExtensions::Equals("AnyTest", 1,2);
    TestExtensions::Equals("AnyTest 2", 2.0,2.0);
    auto testBytes = {(char)0x30, (char)0x82, (char)0x02, (char)0x22, (char)0x30, (char)0x0d, (char)0x06, (char)0x09, (char)0x2a, (char)0x86, (char)0x48, (char)0x86, (char)0xf7, (char)0x0d, (char)0x01, (char)0x01, (char)0x01, (char)0x05, (char)0x00, (char)0x03, (char)0x82, (char)0x02, (char)0x0f, (char)0x00, (char)0x30, (char)0x82, (char)0x02, (char)0x0a, (char)0x02, (char)0x82, (char)0x02, (char)0x01, (char)0x00, (char)0xf2, (char)0xe8, (char)0xe5, (char)0x81, (char)0x32, (char)0x36, (char)0xb8, (char)0xb6, (char)0x3f, (char)0xb5, (char)0xbe, (char)0x76, (char)0x65, (char)0x65, (char)0xd1, (char)0x8f, (char)0x2d, (char)0xc4, (char)0xc5, (char)0xa1, (char)0x91, (char)0x3b, (char)0x8b, (char)0xdc, (char)0x8b, (char)0xf6, (char)0x4f, (char)0x42, (char)0x64, (char)0xd1, (char)0xea, (char)0xdc, (char)0x75, (char)0x6c, (char)0x83, (char)0x0b, (char)0x81, (char)0x1f, (char)0x57, (char)0xeb, (char)0xac, (char)0xe5, (char)0xd0, (char)0x5c, (char)0x6b, (char)0x5f, (char)0x37, (char)0xa8, (char)0x53, (char)0x1c, (char)0x65, (char)0x6b, (char)0x75, (char)0x5e, (char)0xbc, (char)0xd3, (char)0x59, (char)0xd2, (char)0x54, (char)0x17, (char)0xf7, (char)0x69, (char)0x4d, (char)0x23, (char)0x92, (char)0x7e, (char)0x78, (char)0x47, (char)0xf1, (char)0x06, (char)0x04, (char)0x5b, (char)0x55, (char)0x00, (char)0xb1, (char)0xaa, (char)0x82, (char)0x70, (char)0x70, (char)0xc0, (char)0xff, (char)0x3c, (char)0x29, (char)0x4a, (char)0x2f, (char)0xc3, (char)0xff, (char)0x56, (char)0x60, (char)0x4a, (char)0x22, (char)0x12, (char)0xfe, (char)0x10, (char)0xa4, (char)0xe1, (char)0xeb, (char)0x9d, (char)0x82, (char)0xb3, (char)0x76, (char)0x1c, (char)0xa0, (char)0x18, (char)0x4c, (char)0xca, (char)0xcd, (char)0x68, (char)0x40, (char)0x2e, (char)0x6a, (char)0x21, (char)0x2a, (char)0x7b, (char)0x7b, (char)0xc6, (char)0x0b, (char)0x85, (char)0x14, (char)0x19, (char)0x03, (char)0x40, (char)0xe9, (char)0x78, (char)0x54, (char)0xfe, (char)0x97, (char)0xf4, (char)0xe8, (char)0x39, (char)0x45, (char)0x06, (char)0x76, (char)0x8e, (char)0x5e, (char)0x0e, (char)0xdb, (char)0x62, (char)0x41, (char)0x60, (char)0x2b, (char)0xfb, (char)0x1e, (char)0x1a, (char)0x65, (char)0x3a, (char)0x25, (char)0x48, (char)0xba, (char)0xe6, (char)0x73, (char)0x8f, (char)0x35, (char)0xf0, (char)0xfd, (char)0x99, (char)0xe4, (char)0x1d, (char)0xe9, (char)0xbf, (char)0x67, (char)0x8b, (char)0xf4, (char)0x1d, (char)0xfa, (char)0xfa, (char)0x58, (char)0x8e, (char)0xe7, (char)0x1b, (char)0x7b, (char)0xb5, (char)0x7d, (char)0x74, (char)0x90, (char)0x26, (char)0x41, (char)0x88, (char)0xbd, (char)0x4d, (char)0x20, (char)0x69, (char)0x4b, (char)0x4c, (char)0x8a, (char)0xef, (char)0x47, (char)0x87, (char)0xc1, (char)0xf3, (char)0x5b, (char)0x42, (char)0x79, (char)0x04, (char)0xd7, (char)0x9d, (char)0x42, (char)0xa7, (char)0xdf, (char)0xca, (char)0x0d, (char)0xf4, (char)0x19, (char)0x4a, (char)0x8d, (char)0x7c, (char)0x93, (char)0x3f, (char)0x1a, (char)0xa5, (char)0x39, (char)0xef, (char)0xcd, (char)0x6d, (char)0xe5, (char)0x0a, (char)0xe5, (char)0xf0, (char)0x41, (char)0x16, (char)0x96, (char)0x58, (char)0x14, (char)0x99, (char)0x77, (char)0xdc, (char)0x69, (char)0x27, (char)0xc7, (char)0xa6, (char)0x11, (char)0xb4, (char)0xd3, (char)0xa2, (char)0x17, (char)0x23, (char)0x50, (char)0xa0, (char)0xbd, (char)0x06, (char)0x7d, (char)0x5a, (char)0x72, (char)0xa0, (char)0xb1, (char)0xed, (char)0x48, (char)0xd1, (char)0x42, (char)0xfc, (char)0x66, (char)0x3e, (char)0x4a, (char)0x22, (char)0x69, (char)0xac, (char)0xe4, (char)0xee, (char)0x82, (char)0xbc, (char)0x48, (char)0x83, (char)0x81, (char)0x34, (char)0x6e, (char)0x29, (char)0x4b, (char)0x64, (char)0x71, (char)0x37, (char)0x25, (char)0x13, (char)0x28, (char)0x52, (char)0x71, (char)0x5b, (char)0xd5, (char)0x95, (char)0x20, (char)0xa5, (char)0xb4, (char)0x66, (char)0xa7, (char)0x9e, (char)0x06, (char)0x5f, (char)0x2d, (char)0x8e, (char)0x78, (char)0xf5, (char)0x37, (char)0xcf, (char)0xed, (char)0x65, (char)0x84, (char)0xdf, (char)0xda, (char)0x78, (char)0x27, (char)0xa9, (char)0x09, (char)0xaa, (char)0x70, (char)0x73, (char)0x5a, (char)0xc6, (char)0xa9, (char)0xba, (char)0xb7, (char)0xce, (char)0x38, (char)0x2c, (char)0x28, (char)0x4b, (char)0x3e, (char)0xae, (char)0x11, (char)0x3c, (char)0xed, (char)0x94, (char)0xd9, (char)0x2a, (char)0x26, (char)0xd2, (char)0xbc, (char)0xa5, (char)0x19, (char)0x7c, (char)0x3a, (char)0x98, (char)0x0a, (char)0x51, (char)0xdb, (char)0x14, (char)0x99, (char)0xd8, (char)0x4e, (char)0xc3, (char)0x5d, (char)0x0a, (char)0xc9, (char)0x93, (char)0xa9, (char)0xce, (char)0xb0, (char)0x12, (char)0x62, (char)0x6b, (char)0x6b, (char)0x48, (char)0x42, (char)0x42, (char)0x04, (char)0x95, (char)0x29, (char)0x77, (char)0x49, (char)0xaa, (char)0x88, (char)0x2a, (char)0x94, (char)0xcd, (char)0x55, (char)0x7d, (char)0xb6, (char)0xcb, (char)0xb1, (char)0x1e, (char)0x93, (char)0xa9, (char)0xa2, (char)0xba, (char)0x73, (char)0xff, (char)0x2e, (char)0xa6, (char)0xff, (char)0xd6, (char)0x14, (char)0x65, (char)0x3b, (char)0x8c, (char)0x7d, (char)0x0b, (char)0xa7, (char)0xda, (char)0xbd, (char)0x50, (char)0x2c, (char)0x1d, (char)0x2e, (char)0xf1, (char)0xd9, (char)0xf5, (char)0x8a, (char)0x08, (char)0xe9, (char)0x54, (char)0x7d, (char)0x4a, (char)0x25, (char)0xf5, (char)0xb7, (char)0x53, (char)0xd8, (char)0x3f, (char)0xad, (char)0x98, (char)0x5f, (char)0xea, (char)0xa9, (char)0xd5, (char)0x3d, (char)0x13, (char)0x7d, (char)0x26, (char)0x5d, (char)0xab, (char)0x0e, (char)0xa6, (char)0xcd, (char)0xe7, (char)0xc1, (char)0x81, (char)0x0f, (char)0x12, (char)0x8c, (char)0x59, (char)0x77, (char)0xa9, (char)0x67, (char)0xa4, (char)0x37, (char)0xf3, (char)0x8e, (char)0xdf, (char)0xe5, (char)0x5c, (char)0x0c, (char)0x65, (char)0x07, (char)0x93, (char)0xcd, (char)0xb0, (char)0xeb, (char)0x19, (char)0x89, (char)0x6f, (char)0x81, (char)0x90, (char)0x9a, (char)0xf4, (char)0x99, (char)0xb8, (char)0x33, (char)0x35, (char)0xdb, (char)0x40, (char)0x8e, (char)0x85, (char)0x53, (char)0x26, (char)0x4a, (char)0xe9, (char)0x8c, (char)0x5a, (char)0x5d, (char)0x68, (char)0xd5, (char)0x4e, (char)0xff, (char)0x21, (char)0x77, (char)0xb9, (char)0xcb, (char)0xc1, (char)0xaf, (char)0x69, (char)0x69, (char)0x10, (char)0x56, (char)0x6d, (char)0x9e, (char)0xbd, (char)0xe4, (char)0xa4, (char)0x2b, (char)0xd9, (char)0xf9, (char)0x65, (char)0x63, (char)0xb5, (char)0x00, (char)0x48, (char)0xb0, (char)0x04, (char)0xca, (char)0x98, (char)0x10, (char)0x8e, (char)0x2a, (char)0x4f, (char)0x18, (char)0x47, (char)0xef, (char)0x5e, (char)0x26, (char)0x07, (char)0x72, (char)0xf9, (char)0xbe, (char)0x25, (char)0x02, (char)0x03, (char)0x01, (char)0x00, (char)0x01};
    String str = String(testBytes.begin(), testBytes.end());
    std::cout << (StringExtensions::IsValidUTF8(testBytes) ? "💚1" : "💔1") << std::endl;
    std::cout << (StringExtensions::IsValidUTF8("🔺\000@"s) ? "💚2" : "💔2") << std::endl;
    std::cout << (StringExtensions::IsValidUTF8({0x41, 0x40}) ? "💚3" : "💔3") << std::endl;
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
