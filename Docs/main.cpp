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


void DoFetch(const String & idName)
{
    FetchOptions options;
    EMSCRIPTEN_VOID_FUNCTOR_TYPE(1) dataCallback =  [](Emval result){
        JsConsole::Log("Response Data █"s, result);
    };

    Fetch::SendAsync("https://pokeapi.co/api/v2/pokemon/" + idName + "/", options)
    .call<EmscriptenVal>("then", Js::Bind(dataCallback));
}

void ExitEmscripten()
{
    EMVAL_GLOBAL.call<void>("close");
    emscripten_force_exit(99);
}

int main()
{
    DebugExtensions::Debug(true);
    JsConsole::Log("Hello World!!! from . "s + LIB_PRODUCT_NAME + " " + LIB_PRODUCT_VERSION + ".");
    return EXIT_SUCCESS;
}

EMSCRIPTEN_BINDINGS(exports)
{
    function<void>("DoFetch", &DoFetch);
    function<void>("ExitEmscripten", &ExitEmscripten);
    EMSCRIPTEN_EXPORT_ALL_FUNCTORS(3, Insane);

    class_<DebugExtensions>("DebugExtensions")
        .class_function("IsDebug", &DebugExtensions::IsDebug)
        .class_function("Debug", &DebugExtensions::Debug);

    register_vector<uint8_t>("StdVectorUint8");
    register_vector<char>("StdVectorChar");

    class_<ConverterExtensions>("ConverterExtensions")
        .class_function("StdVectorUint8ToString", &ConverterExtensions::StdVectorUint8ToString)
        .class_function("StringToStdVectorUint8", &ConverterExtensions::StringToStdVectorUint8);

    class_<InteropExtensions>("InteropExtensions")
        .class_function("JsUint8ArrayToStdVectorUint8", &InteropExtensions::JsUint8ArrayToStdVectorUint8)
        .class_function("JsStringToStdVectorUint8", &InteropExtensions::JsStringToStdVectorUint8)
        .class_function("StdVectorUint8ToJsUint8Array", &InteropExtensions::StdVectorUint8ToJsUint8Array)
        .class_function("PrintStdVectorUint8", &InteropExtensions::PrintStdVectorUint8);

    enum_<AesCbcPadding>("AesCbcPadding")
        .value("None", AesCbcPadding::None)
        .value("Zeros", AesCbcPadding::Zeros)
        .value("Pkcs7", AesCbcPadding::Pkcs7)
        .value("AnsiX923", AesCbcPadding::AnsiX923);
    EMSCRIPTEN_ENUM_EXTENSIONS_EXPORT(AesCbcPadding);

    enum_<Argon2Variant>("Argon2Variant")
        .value("Argon2d", Argon2Variant::Argon2d)
        .value("Argon2i", Argon2Variant::Argon2i)
        .value("Argon2id", Argon2Variant::Argon2id);
    EMSCRIPTEN_ENUM_EXTENSIONS_EXPORT(Argon2Variant);

    enum_<Base64Encoding>("Base64Encoding")
        .value("Base64", Base64Encoding::Base64)
        .value("UrlSafeBase64", Base64Encoding::UrlSafeBase64)
        .value("FileNameSafeBase64", Base64Encoding::FileNameSafeBase64)
        .value("UrlEncodedBase64", Base64Encoding::UrlEncodedBase64);
    EMSCRIPTEN_ENUM_EXTENSIONS_EXPORT(Base64Encoding);

    enum_<HashAlgorithm>("HashAlgorithm")
        .value("Md5", HashAlgorithm::Md5)
        .value("Sha1", HashAlgorithm::Sha1)
        .value("Sha256", HashAlgorithm::Sha256)
        .value("Sha384", HashAlgorithm::Sha384)
        .value("Sha512", HashAlgorithm::Sha512);
    EMSCRIPTEN_ENUM_EXTENSIONS_EXPORT(HashAlgorithm);

    enum_<RsaKeyPairEncoding>("RsaKeyPairEncoding")
        .value("Ber", RsaKeyPairEncoding::Ber)
        .value("Pem", RsaKeyPairEncoding::Pem)
        .value("Xml", RsaKeyPairEncoding::Xml);
    EMSCRIPTEN_ENUM_EXTENSIONS_EXPORT(RsaKeyPairEncoding);

    enum_<RsaKeyEncoding>("RsaKeyEncoding")
        .value("", RsaKeyEncoding::Unknown)
        .value("", RsaKeyEncoding::BerPublic)
        .value("", RsaKeyEncoding::BerPrivate)
        .value("", RsaKeyEncoding::PemPublic)
        .value("", RsaKeyEncoding::PemPrivate)
        .value("", RsaKeyEncoding::XmlPublic)
        .value("", RsaKeyEncoding::XmlPrivate);
    EMSCRIPTEN_ENUM_EXTENSIONS_EXPORT(RsaKeyEncoding);

    enum_<RsaPadding>("RsaPadding")
        .value("Pkcs1", RsaPadding::Pkcs1)
        .value("OaepSha1", RsaPadding::OaepSha1)
        .value("OaepSha256", RsaPadding::OaepSha256)
        .value("OaepSha384", RsaPadding::OaepSha384)
        .value("OaepSha512", RsaPadding::OaepSha512);
    EMSCRIPTEN_ENUM_EXTENSIONS_EXPORT(RsaPadding);

    class_<HexEncodingExtensions>("HexEncodingExtensions")
        .class_function("EncodeToHex", select_overload<String(const StdVectorUint8 &, const bool &)>(&HexEncodingExtensions::EncodeToHex))
        .class_function("EncodeToHexUsingString", select_overload<String(const String &, const bool &)>(&HexEncodingExtensions::EncodeToHex))
        .class_function("DecodeFromHex", &HexEncodingExtensions::DecodeFromHex);

    class_<Base32EncodingExtensions>("Base32EncodingExtensions")
        .class_function("DecodeFromBase32", &Base32EncodingExtensions::DecodeFromBase32)
        .class_function("EncodeToBase32", select_overload<String(const StdVectorUint8 &, const bool &, const bool &)>(&Base32EncodingExtensions::EncodeToBase32))
        .class_function("EncodeToBase32UsingString", select_overload<String(const String &, const bool &, const bool &)>(&Base32EncodingExtensions::EncodeToBase32));

    class_<Base64EncodingExtensions>("Base64EncodingExtensions")
        .class_function("DecodeFromBase64", &Base64EncodingExtensions::DecodeFromBase64)
        .class_function("EncodeToBase64", select_overload<String(const StdVectorUint8 &, const size_t &, const bool &)>(&Base64EncodingExtensions::EncodeToBase64))
        .class_function("EncodeToBase64UsingString", select_overload<String(const String &, const size_t &, const bool &)>(&Base64EncodingExtensions::EncodeToBase64))
        .class_function("EncodeToUrlSafeBase64", select_overload<String(const StdVectorUint8 &)>(&Base64EncodingExtensions::EncodeToUrlSafeBase64))
        .class_function("EncodeToUrlSafeBase64UsingString", select_overload<String(const String &)>(&Base64EncodingExtensions::EncodeToUrlSafeBase64))
        .class_function("EncodeToFilenameSafeBase64", select_overload<String(const StdVectorUint8 &)>(&Base64EncodingExtensions::EncodeToFilenameSafeBase64))
        .class_function("EncodeToFilenameSafeBase64UsingString", select_overload<String(const String &)>(&Base64EncodingExtensions::EncodeToFilenameSafeBase64))
        .class_function("EncodeToUrlEncodedBase64", select_overload<String(const StdVectorUint8 &)>(&Base64EncodingExtensions::EncodeToUrlEncodedBase64))
        .class_function("EncodeToUrlEncodedBase64UsingString", select_overload<String(const String &)>(&Base64EncodingExtensions::EncodeToUrlEncodedBase64))
        .class_function("EncodeBase64ToUrlSafeBase64", &Base64EncodingExtensions::EncodeBase64ToUrlSafeBase64)
        .class_function("EncodeBase64ToFilenameSafeBase64", &Base64EncodingExtensions::EncodeBase64ToFilenameSafeBase64)
        .class_function("EncodeBase64ToUrlEncodedBase64", &Base64EncodingExtensions::EncodeBase64ToUrlEncodedBase64);

    class_<IEncoder>("IEncoder");
    class_<DeserializeResolver<IEncoder>>("DeserializeResolver<IEncoder>");

    class_<HexEncoder, base<IEncoder>>("HexEncoder")
        .constructor<const bool &>()
        .function("Encode", select_overload<String(const StdVectorUint8 &) const>(&HexEncoder::Encode))
        .function("EncodeUsingString", select_overload<String(const String &) const>(&HexEncoder::Encode))
        .function("Decode", &HexEncoder::Decode)
        .function("Serialize", &HexEncoder::Serialize)
        .function("Clone", &HexEncoder::Clone)
        .class_function("DefaultInstance", &HexEncoder::DefaultInstance)
        .class_function("Deserialize", &HexEncoder::Deserialize)
        .class_function("CreateInstance", &HexEncoder::CreateInstance)
        .class_function("DefaultDeserializeResolver", &HexEncoder::DefaultDeserializeResolver);

    class_<Base32Encoder, base<IEncoder>>("Base32Encoder")
        .constructor<const bool &, const bool &>()
        .function("Encode", select_overload<String(const StdVectorUint8 &) const>(&Base32Encoder::Encode))
        .function("EncodeUsingString", select_overload<String(const String &) const>(&Base32Encoder::Encode))
        .function("Decode", &Base32Encoder::Decode)
        .function("Serialize", &Base32Encoder::Serialize)
        .function("Clone", &Base32Encoder::Clone)
        .class_function("DefaultInstance", &Base32Encoder::DefaultInstance)
        .class_function("Deserialize", &Base32Encoder::Deserialize)
        .class_function("CreateInstance", &Base32Encoder::CreateInstance)
        .class_function("DefaultDeserializeResolver", &Base32Encoder::DefaultDeserializeResolver);

    class_<Base64Encoder, base<IEncoder>>("Base64Encoder")
        .constructor<const size_t &, const bool &, const Base64Encoding &>()
        .function("Encode", select_overload<String(const StdVectorUint8 &) const>(&Base64Encoder::Encode))
        .function("EncodeUsingString", select_overload<String(const String &) const>(&Base64Encoder::Encode))
        .function("Decode", &Base64Encoder::Decode)
        .function("Serialize", &Base64Encoder::Serialize)
        .function("Clone", &Base64Encoder::Clone)
        .class_function("DefaultInstance", &Base64Encoder::DefaultInstance)
        .class_function("Deserialize", &Base64Encoder::Deserialize)
        .class_function("CreateInstance", &Base64Encoder::CreateInstance)
        .class_function("DefaultDeserializeResolver", &Base64Encoder::DefaultDeserializeResolver);

    class_<HashExtensions>("HashExtensions")
        .class_function("ComputeHash", select_overload<StdVectorUint8(const StdVectorUint8 &, const HashAlgorithm &)>(&HashExtensions::ComputeHash))
        .class_function("ComputeHmac", select_overload<StdVectorUint8(const StdVectorUint8 &, const StdVectorUint8 &, const HashAlgorithm &)>(&HashExtensions::ComputeHmac))
        .class_function("ComputeArgon2", select_overload<StdVectorUint8(const StdVectorUint8 &, const StdVectorUint8 &, const size_t &, const size_t &, const size_t &, const Argon2Variant &, const size_t &)>(&HashExtensions::ComputeArgon2))
        .class_function("ComputeScrypt", select_overload<StdVectorUint8(const StdVectorUint8 &, const StdVectorUint8 &, const size_t &, const size_t &, const size_t &, const size_t &)>(&HashExtensions::ComputeScrypt))
        .class_function("ComputeHashUsingString", select_overload<StdVectorUint8(const String &, const HashAlgorithm &)>(&HashExtensions::ComputeHash))
        .class_function("ComputeHmacUsingString", select_overload<StdVectorUint8(const String &, const String &, const HashAlgorithm &)>(&HashExtensions::ComputeHmac))
        .class_function("ComputeArgon2UsingString", select_overload<StdVectorUint8(const String &, const String &, const size_t &, const size_t &, const size_t &, const Argon2Variant &, const size_t &)>(&HashExtensions::ComputeArgon2))
        .class_function("ComputeScryptUsingString", select_overload<StdVectorUint8(const String &, const String &, const size_t &, const size_t &, const size_t &, const size_t &)>(&HashExtensions::ComputeScrypt))
        .class_function("ComputeEncodedHash", select_overload<String(const StdVectorUint8 &, const IEncoder *, const HashAlgorithm &)>(&HashExtensions::ComputeEncodedHash), allow_raw_pointers())
        .class_function("ComputeEncodedHmac", select_overload<String(const StdVectorUint8 &, const StdVectorUint8 &, const IEncoder *, const HashAlgorithm &)>(&HashExtensions::ComputeEncodedHmac), allow_raw_pointers())
        .class_function("ComputeEncodedArgon2", select_overload<String(const StdVectorUint8 &, const StdVectorUint8 &, const IEncoder *, const size_t &, const size_t &, const size_t &, const Argon2Variant &, const size_t &)>(&HashExtensions::ComputeEncodedArgon2), allow_raw_pointers())
        .class_function("ComputeEncodedScrypt", select_overload<String(const StdVectorUint8 &, const StdVectorUint8 &, const IEncoder *, const size_t &, const size_t &, const size_t &, const size_t &)>(&HashExtensions::ComputeEncodedScrypt), allow_raw_pointers())
        .class_function("ComputeEncodedHashUsingString", select_overload<String(const String &, const IEncoder *, const HashAlgorithm &)>(&HashExtensions::ComputeEncodedHash), allow_raw_pointers())
        .class_function("ComputeEncodedHmacUsingString", select_overload<String(const String &, const String &, const IEncoder *, const HashAlgorithm &)>(&HashExtensions::ComputeEncodedHmac), allow_raw_pointers())
        .class_function("ComputeEncodedArgon2UsingString", select_overload<String(const String &, const String &, const IEncoder *, const size_t &, const size_t &, const size_t &, const Argon2Variant &, const size_t &)>(&HashExtensions::ComputeEncodedArgon2), allow_raw_pointers())
        .class_function("ComputeEncodedScryptUsingString", select_overload<String(const String &, const String &, const IEncoder *, const size_t &, const size_t &, const size_t &, const size_t &)>(&HashExtensions::ComputeEncodedScrypt), allow_raw_pointers());

    class_<AesExtensions>("AesExtensions")
        .class_function("EncryptAesCbc", select_overload<StdVectorUint8(const StdVectorUint8 &, const StdVectorUint8 &, const AesCbcPadding &)>(&AesExtensions::EncryptAesCbc))
        .class_function("EncryptAesCbcUsingString", select_overload<StdVectorUint8(const String &, const String &, const AesCbcPadding &)>(&AesExtensions::EncryptAesCbc))
        .class_function("EncryptEncodedAesCbc", select_overload<String(const StdVectorUint8 &, const StdVectorUint8 &, const IEncoder *, const AesCbcPadding &)>(&AesExtensions::EncryptEncodedAesCbc), allow_raw_pointers())
        .class_function("EncryptEncodedAesCbcUsingString", select_overload<String(const String &, const String &, const IEncoder *, const AesCbcPadding &)>(&AesExtensions::EncryptEncodedAesCbc), allow_raw_pointers())
        .class_function("DecryptAesCbc", select_overload<StdVectorUint8(const StdVectorUint8 &, const StdVectorUint8 &, const AesCbcPadding &)>(&AesExtensions::DecryptAesCbc))
        .class_function("DecryptAesCbcUsingString", select_overload<StdVectorUint8(const StdVectorUint8 &, const String &, const AesCbcPadding &)>(&AesExtensions::DecryptAesCbc))
        .class_function("DecryptEncodedAesCbc", select_overload<StdVectorUint8(const String &, const StdVectorUint8 &, const IEncoder *, const AesCbcPadding &)>(&AesExtensions::DecryptEncodedAesCbc), allow_raw_pointers())
        .class_function("DecryptEncodedAesCbcUsingString", select_overload<StdVectorUint8(const String &, const String &, const IEncoder *, const AesCbcPadding &)>(&AesExtensions::DecryptEncodedAesCbc), allow_raw_pointers());

    class_<RsaKeyPair>("RsaKeyPair")
        .constructor<const String &, const String &>()
        .function("GetPublicKey", &RsaKeyPair::GetPublicKey)
        .function("GetPrivateKey", &RsaKeyPair::GetPrivateKey)
        .function("Serialize", &RsaKeyPair::Serialize)
        .class_function("Deserialize", &RsaKeyPair::Deserialize);

    class_<RsaExtensions>("RsaExtensions")
        .class_function("CreateRsaKeyPair", &RsaExtensions::CreateRsaKeyPair)
        .class_function("GetRsaKeyEncoding", &RsaExtensions::GetRsaKeyEncoding)
        .class_function("ValidateRsaPublicKey", &RsaExtensions::ValidateRsaPublicKey)
        .class_function("ValidateRsaPrivateKey", &RsaExtensions::ValidateRsaPrivateKey)
        .class_function("EncryptRsa", select_overload<StdVectorUint8(const StdVectorUint8 &, const String &, const RsaPadding &)>(&RsaExtensions::EncryptRsa))
        .class_function("EncryptRsaUsingString", select_overload<StdVectorUint8(const String &, const String &, const RsaPadding &)>(&RsaExtensions::EncryptRsa))
        .class_function("EncryptEncodedRsa", select_overload<String(const StdVectorUint8 &, const String &, const IEncoder *, const RsaPadding &)>(&RsaExtensions::EncryptEncodedRsa), allow_raw_pointers())
        .class_function("EncryptEncodedRsaUsingString", select_overload<String(const String &, const String &, const IEncoder *, const RsaPadding &)>(&RsaExtensions::EncryptEncodedRsa), allow_raw_pointers())
        .class_function("DecryptRsa", select_overload<StdVectorUint8(const StdVectorUint8 &, const String &, const RsaPadding &)>(&RsaExtensions::DecryptRsa))
        .class_function("DecryptEncodedRsa", select_overload<StdVectorUint8(const String &, const String &, const IEncoder *, const RsaPadding &)>(&RsaExtensions::DecryptEncodedRsa), allow_raw_pointers());

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

    value_object<Browser::MimeType>("Mime")
        .field("Description", &Browser::MimeType::Description)
        .field("Description", &Browser::MimeType::Suffixes)
        .field("Type", &Browser::MimeType::Type);
    register_vector<Browser::MimeType>("VectorB");
    value_object<Browser::Plugin>("PluginClass").field("Name", &Browser::Plugin::Name).field("MimeTypes", &Browser::Plugin::MimeTypes);
    register_vector<Browser::Plugin>("VectorA");
}
