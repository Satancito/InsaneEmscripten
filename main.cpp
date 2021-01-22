#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#include <iostream>
#include <Insane/InsaneEmscripten.h>
#include <Insane/InsaneCryptography.h>
#include <Insane/InsaneCore.h>

void TestBrowser()
{
    USING_EMSCRIPTEN;
    USING_INSANE_EMSCRIPTEN;
    auto x = "Non std::string literal";
    //Console::Log("Non std::string literal");
    //Console::Log("GOOOO"s);
    //Console::Log(val("GOOOO"));
    //Console::Log<val>(val("GOOOO"));
    //Console::Log<String>("GOOOO"s);
    auto vv = val(10.8f);
    auto vv2 = val(10.8);
    auto vv3 = 10.8_val;
    auto vv4 = 10.8_val;

    val::global("console").call<void>("log", vv);
    val::global("console").call<void>("log", vv2);
    val::global("console").call<void>("log", vv3);
    val::global("console").call<void>("log", vv4);
    val::global("console").call<void>("log", val(10.8f));
    val::global("console").call<void>("log", val(10.8));
    Console::Log(10.8_val);
    Console::Log(10.8_val);

    Console::Log("From val float = "s, val(32.8f));
    Console::Log("From val double = "s, val(32.8));
    Console::Log("From float = "s, 32.50f);
    Console::Log("From double = "s, 32.50);
    Console::Log("From Suffix float = "s, 10.8_val);
    Console::Log("From Suffix double = "s, 10.8_val);
    Console::Log(123232323232_val);
    Console::Log(123232323232_val);
    Console::Log(1_valb);
    Console::Log(0_valb);
    Console::Log("hola %s %s %d %s %s"s, "helloWorld"s, "Joma"s, 1, 1990_val, "hola"s);
    Console::Log("From Suffix std::string = "s, "Hello from Suffix\n\n2020"_val);
    Console::Log("From Suffix char = "s, '@'_val);
    val::global("console").call<void>("log", val(5));
    Console::Log(Browser::GetUserAgent<val>());
    Console::Log(Browser::GetUserAgent<String>());
    Console::Log(Browser::GetUserAgent());
    Console::Log("██"_val);
}

void TestLocalStorage()
{
    USING_EMSCRIPTEN;
    USING_INSANE_EMSCRIPTEN;
    LocalStorage::SetValue<String>("ValorCifrado", "HelloWorld", "password");
    LocalStorage::SetValue<String>("ValorNoCifrado", "HelloWorld");
    Console::Log(LocalStorage::GetValue<String>("ValorCifrado"s, "password"s));
    Console::Log(LocalStorage::GetValue<String>("ValorNoCifrado"s));

    LocalStorage::SetValue<String>("ValorCifrado"s, "HelloWorld!!!"s, "password"s);
    LocalStorage::SetValue<String>("ValorNoCifrado", "HelloWorld!!!");
    Console::Log(LocalStorage::GetValue<String>("ValorCifrado"s, "password"s));
    Console::Log(LocalStorage::GetValue<String>("ValorNoCifrado"s));

    Console::Log(LocalStorage::GetValue<>("ValorCifrado"s, "password15"s));
    Console::Log(LocalStorage::GetValue<String>(val("ValorCifradoxxx"s)));

    LocalStorage::SetValue(val::undefined(), val::undefined(), val("password"s));
    LocalStorage::SetValue(val::null(), val::null(), val("password"s));
    LocalStorage::SetValue(val::array(), val::undefined());
    LocalStorage::SetValue(val::global(), val::null());
}

void HashManagerTests()
{
    USING_INSANE_EMSCRIPTEN;
    USING_INSANE_CRYPTO;
    Console::Log(u8"██████ HashManagerTests ██████"s);
    String str = "A";
    str = HashManager::ToBase64(str, 0, true);
    Console::Log(str);
    str = HashManager::FromBase64(str);
    Console::Log(str);

    str = HashManager::ToBase64(str, 0, false);
    Console::Log(str);
    str = HashManager::FromBase64(str);
    Console::Log(str);
}

void RsaManagerTests()
{
    USING_EMSCRIPTEN;
    USING_INSANE_CRYPTO;
    USING_INSANE_EMSCRIPTEN;
    Console::Log(u8"██████ RsaManagerTests ██████"s);
    String data = u8"Hello world!!! 🐶"s;
    String json;
    UInt32 keySize = 4096;

    RsaKeyPair keyPair = RsaManager::CreateKeyPair(keySize, RsaKeyEncoding::Ber, false);
    Console::Log(u8"█ BER \nPublic: \n%s\nPrivate: \n%s"s, keyPair.PublicKey(), keyPair.PrivateKey());
    json = keyPair.Serialize();
    Console::Log(u8"Serialized:\n"s, json);
    keyPair = RsaKeyPair::Deserialize(json);
    String encrypted = RsaManager::EncryptToBase64(data, keyPair.PublicKey());
    Console::Log(u8"Encrypted: %s"s, encrypted);
    Console::Log(u8"Decrypted: %s"s, RsaManager::DecryptFromBase64(encrypted, keyPair.PrivateKey()));
    Console::Log(u8"███"s);

    keyPair = RsaManager::CreateKeyPair(keySize, RsaKeyEncoding::Pem, false);
    Console::Log(u8"█ PEM \nPublic: \n%s\nPrivate: \n%s"s, keyPair.PublicKey(), keyPair.PrivateKey());
    json = keyPair.Serialize();
    Console::Log(u8"Serialized:\n"s, json);
    keyPair = RsaKeyPair::Deserialize(json);
    encrypted = RsaManager::EncryptToBase64(data, keyPair.PublicKey());
    Console::Log(u8"Encrypted: %s"s, encrypted);
    Console::Log(u8"Decrypted: %s"s, RsaManager::DecryptFromBase64(encrypted, keyPair.PrivateKey()));

    keyPair = RsaManager::CreateKeyPair(keySize, RsaKeyEncoding::Json, true);
    Console::Log(u8"█ JSON INDENTED \nPublic: \n@%s@\nPrivate: \n@%s@"s, keyPair.PublicKey(), keyPair.PrivateKey());
    json = keyPair.Serialize();
    Console::Log(u8"Serialized:\n"s, json);
    keyPair = RsaKeyPair::Deserialize(json);
    encrypted = RsaManager::EncryptToBase64(data, keyPair.PublicKey());
    Console::Log(u8"Encrypted: %s"s, encrypted);
    Console::Log(u8"Decrypted: %s"s, RsaManager::DecryptFromBase64(encrypted, keyPair.PrivateKey()));

    keyPair = RsaManager::CreateKeyPair(keySize, RsaKeyEncoding::Json, false);
    Console::Log(u8"█ JSON UNINDENTED \nPublic: \n%s\nPrivate: \n%s"s, keyPair.PublicKey(), keyPair.PrivateKey());
    json = keyPair.Serialize();
    Console::Log(u8"Serialized:\n"s, json);
    keyPair = RsaKeyPair::Deserialize(json);
    encrypted = RsaManager::EncryptToBase64(data, keyPair.PublicKey());
    Console::Log(u8"Encrypted: %s"s, encrypted);
    Console::Log(u8"Decrypted: %s"s, RsaManager::DecryptFromBase64(encrypted, keyPair.PrivateKey()));

    keyPair = RsaManager::CreateKeyPair(keySize, RsaKeyEncoding::Xml, true);
    Console::Log(u8"█ XML INDENTED \nPublic: \n%s\nPrivate: \n%s"s, keyPair.PublicKey(), keyPair.PrivateKey());
    json = keyPair.Serialize();
    Console::Log(u8"Serialized:\n"s, json);
    keyPair = RsaKeyPair::Deserialize(json);
    encrypted = RsaManager::EncryptToBase64(data, keyPair.PublicKey());
    Console::Log(u8"Encrypted: %s"s, encrypted);
    Console::Log(u8"Decrypted: %s"s, RsaManager::DecryptFromBase64(encrypted, keyPair.PrivateKey()));

    keyPair = RsaManager::CreateKeyPair(keySize, RsaKeyEncoding::Xml, false);
    Console::Log(u8"█ XML \nPublic: \n%s\nPrivate: \n%s"s, keyPair.PublicKey(), keyPair.PrivateKey());
    json = keyPair.Serialize();
    Console::Log(u8"Serialized:\n"s, json);
    keyPair = RsaKeyPair::Deserialize(json);
    encrypted = RsaManager::EncryptToBase64(data, keyPair.PublicKey());
    Console::Log(u8"Encrypted: %s"s, encrypted);
    Console::Log(u8"Decrypted: %s"s, RsaManager::DecryptFromBase64(encrypted, keyPair.PrivateKey()));
}

void TestScrypt()
{
    USING_INSANE_CRYPTO;
    USING_INSANE_EMSCRIPTEN;
    //Console::Log("Scrypt hash: %s"s, HashManager::ToBase64Scrypt(u8"mypassword"));//https://8gwifi.org/scrypt.jsp
}

#define INSANE_PROPERTY_NAMES_KEY u8"InsanePropertyNamesKey"s
#define INSANE_KEY u8"InsaneKey"s


#define JSON_RSA_PUBLIC_AND_PRIVATE_KEY_REGEX_PATTERN_STRING u8R"((\s*\{(?:\s*"Modulus"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()|\s*"Exponent"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()|\s*"P"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()|\s*"Q"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()|\s*"DP"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()|\s*"DQ"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()|\s*"InverseQ"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()|\s*"D"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()){8}\s*\2\3\4\5\6\7\8\9\}\s*)|(\s*\{(?:\s*"Modulus"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()|\s*"Exponent"\s*:\s*"[a-zA-Z\d\+\/\\]+={0,2}"\s*\,()){2}\s*\11\12\}\s*))" //https://regex101.com/r/v5lUWw/1 //Add final Comma.
int main()
{

    USING_EMSCRIPTEN;
    USING_INSANE_EMSCRIPTEN;
    USING_INSANE_CRYPTO;
    USING_INSANE_STR;
    USING_INSANE_EXCEPTION;
    //std::decay<const char[1]>::type x;
    const char xxx[] = "hello";
    char yyy[] = "world";
    char *zzz = "Joma";
    const char *www = "Satancito";
    auto aaa = "HelloWorld!!!";
    Console::Log("Param1 = %s Param2 = %s Param3 = %s Param4 = %s", www, xxx, yyy, zzz, aaa, 1, true, 540.00f, 640.00);
    //LoadScriptAsync(val("Libs/client.min.js"));
    // val x = JsManager::LoadScript(val("Libs/client.min.js"));
    // EMSCRIPTEN_VOID_FUNCTOR_TYPE(1) fx = [](val id){
    //     Console::Log(id);
    // };
    // x = x.call<val>("then", JsManager::Bind(fx));
    // Console::Log(x);

    //Browser1 = Bowser
    //Browser2 = UAParser
    String propertyKey = u8"PropertyKey";
    val::global().set("VariableData", "1"s);
    String data = "HelloWorld 🐶🐶🐶";
    String key = "12345";
    String enc = AesManager::EncryptToBase64(data, key);
    Console::Log("Encrypted: "s, enc);
    try
    {
        Console::Log("Decrypted: "s, AesManager::DecryptFromBase64(enc, key));
    }

    catch (ExceptionBase &ex)
    {
        std::cerr << "ExceptionBase" << ex.Message() << '\n';
    }

    // TestBrowser();
    //TestLocalStorage();

    // Console::Log(Browser::GetNameAsync());
    // Console::Log(Browser::GetNameAsync(emscripten::val::null()));
    // Console::Log(Browser::GetNameAsync(emscripten::val::undefined()));

    // Console::Log(Browser::GetAvailableScreenHeight<int>());
    // Console::Log(Browser::GetAvailableScreenWidth<int>());
    // Insane::Emscripten::OrientationType orientation = Browser::GetAvailableScreenOrientation<Insane::Emscripten::OrientationType>();
    // Console::Log(static_cast<int>(orientation));
    // Console::Log(Browser::GetScreenHeight<int>());
    // Console::Log(Browser::GetScreenWidth<int>());
    // Insane::Emscripten::OrientationType orientation2 = Browser::GetScreenOrientation<Insane::Emscripten::OrientationType>();
    // Console::Log(static_cast<int>(orientation2));
    // Console::Log(Browser::GetViewportHeight<int>());
    // Console::Log(Browser::GetViewportWidth<int>());
    // Insane::Emscripten::OrientationType orientation3 = Browser::GetViewportOrientation<Insane::Emscripten::OrientationType>();
    // Console::Log(static_cast<int>(orientation3));
    // Console::Log(u8"BrowserLang: "s, Browser::GetLanguage());
    // std::vector<String> langs = Browser::GetLanguages<std::vector<String>>();
    // Console::Log(u8"█ Languages"s);
    // for (auto value : langs)
    // {
    //     Console::Log("- Language: "s, value);
    // }
    // Console::Log(u8"BrowserDeviceMemory: "s, Browser::GetDeviceMemory());
    // Console::Log(u8"BrowserMaxTouchPoints: "s, Browser::GetMaxTouchPoints());
    // Console::Log(u8"BrowserHardwareConcurrency: "s, Browser::GetHardwareConcurrency());
    // Console::Log(u8"█ BrowserPlugins"s);
    // for (Browser::Plugin value : Browser::GetPlugins<std::vector<Browser::Plugin>>())
    // {
    //     Console::Log(u8"- Plugin: %s"s, val(value.Name));
    //     for (auto mime : value.MimeTypes)
    //     {
    //         Console::Log(u8"    MimeType { Description: %s, Type: %s, Suffixes: %s }"s, mime.Description, mime.Type, mime.Suffixes);
    //     }
    // }

    // Console::Log(u8"█ BrowserMimeTypes"s);
    // for (auto mime : Browser::GetMimeTypes<std::vector<Browser::MimeType>>())
    // {
    //     Console::Log(u8"- MimeType { Description: %s, Type: %s, Suffixes: %s }"s, mime.Description, mime.Type, mime.Suffixes);
    // }

    // Console::Log(u8"Browser::HasCookiesSupport"s, Browser::HasCookiesSupport());
    // Console::Log(u8"Browser::GetDoNotTrack"s, Browser::GetDoNotTrack());
    //TestScrypt();
    //HashManagerTests();
    //RsaManagerTests();
    //     String rsaKey = u8R"({
    // 	"Modulus": "sPEPEJ373N7UuKpUqPHc0CQxcH2M1JqrBaE+O2Pz6AyDeWxH1/NQxbK1MCFJYAMpWGiVTPBy2zJhta9Fu0xsXjrr+pn3QxWsqCrPjQGW62cdZrRJYid7cGEaxMNTz8GmovoP/YYhz4gRzIhKj363uHQbC7th9bWV6Kv50fAzkE6Ti1E8JxMYq728AmmE/OXLXXEvTjHp/T5g395LkVPuwnRbTyNUvX/fPEKZD9bnMPyKiNWUZRmy6xYFaICkgI+SyHWQMWNIZ1mxHDmKIlsm+qEhsnkSgyH0R19NjeAOLhMsYU4HUpDJR/WuvAdwq0C4M8s+CRR5hfWmcpmmTF347p9VuGnoGGxmEbGU9KCTj9X+MFmgGGD5bQXbl7TieXAYl1l1+knOdIeKy8FOC6jqXIirshU+dLbdWdcQuDYe9AKZFN1zfkXTS6AS77ET/qj0cnRbqXRGLHstPcZ/AFUkefy+VucSNpNNgdqfAlUa2S+gapQar5IWACMZkPFX5qiacoJcDd9PIzmKtnyUqQr6r25CBqdzxKUgTsMeetZa9awoyAyY8qh1AhR00AA5zFNa6jesB4NfCHXy69y5g7jh68Zfl81V0bKw1PCyWdn9ycb+UwcYO6Lw+m+skzlNBbmI6FDJQUvACBlyz65BlogPiw8gfyUaFlyHHRTrvl6g3n0=",
    // 	"Exponent": "AQAB"
    // })";
    //     rsaKey = Strings::ReplaceLastOf(rsaKey, u8"\"", u8"\",");
    //     if (Strings::IsMatch(rsaKey, JSON_RSA_PUBLIC_AND_PRIVATE_KEY_REGEX_PATTERN_STRING))
    //     {
    //         Console::Log("IS JSON"s);
    //     }

    // Insane::Core::Console::Pause();
    // Insane::Core::Console::PauseAny();
    // Insane::Core::Console::Clear();
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
    EMSCRIPTEN_EXPORT_ALL_FUNCTORS(5);
    value_object<Browser::MimeType>(u8"Mime")
        .field("Description", &Insane::Emscripten::Browser::MimeType::Description)
        .field("Description", &Insane::Emscripten::Browser::MimeType::Suffixes)
        .field("Type", &Insane::Emscripten::Browser::MimeType::Type);
    register_vector<Browser::MimeType>("VectorB");
    value_object<Browser::Plugin>(u8"PluginClass").field("Name", &Browser::Plugin::Name).field("MimeTypes", &Browser::Plugin::MimeTypes);
    register_vector<Browser::Plugin>("VectorA");

    // function<bool>(u8"SetVolatileValue", &Insane::Emscripten::LocalStorage::SetVolatileValue);
    // function<String>(u8"GetVolatileValue", &Insane::Emscripten::LocalStorage::GetVolatileValue);
    // function<void>(u8"RemoveValue", &Insane::Emscripten::LocalStorage::RemoveValue<val>);
    // function<void>(u8"Clear", &Insane::Emscripten::LocalStorage::Clear);
    // function<val>(u8"BrowserGetName", &Insane::Emscripten::Browser::GetName<val>);
    // function<val>(u8"IsBrave", &IsBrave);
    // function<String>(u8"IsBraveStr", &IsBraveStr);
    // register_vector<String>("VectorString");
    // function<void>(u8"RemoveVolatileValues", &Insane::Emscripten::LocalStorage::RemoveVolatileValues);
}


//TODO: IMPLEMENTAR OPERADOR = de Xtring