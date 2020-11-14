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
    LocalStorage::SetValue("ValorCifrado", "HelloWorld", "password");
    LocalStorage::SetValue("ValorNoCifrado", "HelloWorld");
    Console::Log(LocalStorage::GetValue("ValorCifrado", "password"));
    Console::Log(LocalStorage::GetValue("ValorNoCifrado"));

    auto x = LocalStorage::SetValue("ValorCifrado", "HelloWorld!!!", "password");
    x = LocalStorage::SetValue("ValorNoCifrado", "HelloWorld!!!");
    Console::Log(LocalStorage::GetValue("ValorCifrado", "password"));
    Console::Log(LocalStorage::GetValue("ValorNoCifrado"));
    Console::Log(LocalStorage::GetValue("ValorCifrado", "password15"));
    Console::Log(LocalStorage::GetValue("ValorCifradoxxx"));
}

#define INSANE_PROPERTY_NAMES_KEY u8"InsanePropertyNamesKey"s
#define INSANE_KEY u8"InsaneKey"s

int main()
{
    USING_EMSCRIPTEN;
    USING_INSANE_EMSCRIPTEN;
    USING_INSANE_CRYPTO;
    USING_INSANE_STR;
    USING_INSANE_EXCEPTION;
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
    catch (const ExceptionBase &e)
    {
        std::cerr << e.Message() << "- █ - " << e.Code() << '\n';
    }

    //Js::Init(propertyKey);

    // Js::LoadScript(val("Libs/ClientJS/client.min1.js"));
    // Js::LoadScript(val("Libs/ClientJS/client.min1.js"));
    // Js::LoadScript(val("Libs/ClientJS/client.min1.js"));
    // Js::LoadScript(val("Libs/ClientJS/client.min.js"));
    // Js::LoadScript(val("Libs/ClientJS/client.min.js"));
    // Js::LoadScript(val("Libs/ClientJS/client.min.js"));
    // TestBrowser();
    // TestLocalStorage();
    Console::Log(Browser::GetNameAsync());
    Console::Log(Browser::GetNameAsync(emscripten::val::null()));
    Console::Log(Browser::GetNameAsync(emscripten::val::undefined()));

    Console::Log(Browser::GetAvailableScreenHeight<int>());
    Console::Log(Browser::GetAvailableScreenWidth<int>());
    Insane::Emscripten::OrientationType orientation = Browser::GetAvailableScreenOrientation<Insane::Emscripten::OrientationType>();
    Console::Log(static_cast<int>(orientation));
    Console::Log(Browser::GetScreenHeight<int>());
    Console::Log(Browser::GetScreenWidth<int>());
    Insane::Emscripten::OrientationType orientation2 = Browser::GetScreenOrientation<Insane::Emscripten::OrientationType>();
    Console::Log(static_cast<int>(orientation2));
    Console::Log(Browser::GetViewportHeight<int>());
    Console::Log(Browser::GetViewportWidth<int>());
    Insane::Emscripten::OrientationType orientation3 = Browser::GetViewportOrientation<Insane::Emscripten::OrientationType>();
    Console::Log(static_cast<int>(orientation3));
    Console::Log(u8"BrowserLang: "s, Browser::GetLanguage());
    std::vector<String> langs = Browser::GetLanguages<std::vector<String>>();
    Console::Log(u8"█ Languages"s);
    for (auto value : langs)
    {
        Console::Log("- Language: "s, value);
    }
    Console::Log(u8"BrowserDeviceMemory: "s, Browser::GetDeviceMemory());
    Console::Log(u8"BrowserMaxTouchPoints: "s, Browser::GetMaxTouchPoints());
    Console::Log(u8"BrowserHardwareConcurrency: "s, Browser::GetHardwareConcurrency());
    Console::Log(u8"█ BrowserPlugins"s);
    for (Browser::Plugin value : Browser::GetPlugins<std::vector<Browser::Plugin>>())
    {
        Console::Log(u8"- Plugin: %s"s, val(value.Name));
        for (auto mime : value.MimeTypes)
        {
            Console::Log(u8"    MimeType { Description: %s, Type: %s, Suffixes: %s }"s, mime.Description, mime.Type, mime.Suffixes);
        }
    }

    Console::Log(u8"█ BrowserMimeTypes"s);
    for (auto mime : Browser::GetMimeTypes<std::vector<Browser::MimeType>>())
    {
        Console::Log(u8"- MimeType { Description: %s, Type: %s, Suffixes: %s }"s, mime.Description, mime.Type, mime.Suffixes);
    }
    
    Console::Log(u8"Browser::HasCookiesSupport"s, Browser::HasCookiesSupport());
    Console::Log(u8"Browser::GetDoNotTrack"s, Browser::GetDoNotTrack());

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
    function<val>(u8"PromiseResolve", &Promise::Resolve);
    function<val>(u8"PromiseReject", &Promise::Reject);
    function<val>(u8"OperatorAdd", &Operator::Add);
    function<val>(u8"OperatorSubtract", &Operator::Subtract);
    function<val>(u8"OperatorMultiply", &Operator::Multiply);
    function<val>(u8"OperatorDivide", &Operator::Divide);
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
