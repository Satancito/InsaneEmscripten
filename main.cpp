#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#include <iostream>
#include <Insane/InsaneEmscripten.h>
#include <Insane/InsaneCryptography.h>

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

    Console::Log("From Suffix float = "s, val(32.8f));
    Console::Log("From Suffix double = "s, val(32.8));
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
    Console::Log(Browser::GetName());
    Console::Log(Browser::GetName<String>());
}

emscripten::val IsBrave()
{
    using namespace emscripten;
    bool isBrave = !!val::global()[u8"navigator"][u8"brave"] &&
                   !!(val::global()[u8"navigator"][u8"brave"].call<val>(u8"isBrave").await());
    if (isBrave)
    {
        std::cout << "It's Brave." << std::endl;
        return val(u8"Brave"s);
    }
    else
    {
        std::cout << "It's other." << std::endl;
        return val(u8"Unknown"s);
    }
}

// emscripten::val IsBrave()
// {
//     using namespace emscripten;
//     bool isBrave = !!val::global()[u8"navigator"][u8"brave"] && !!(val::global()[u8"navigator"][u8"brave"].call<val>(u8"isBrave").await());
//     if(isBrave)
//     {
//         std::cout<< "It's Brave." << std::endl;
//         return val(u8"Brave"s);
//     }
//     else
//     {
//         std::cout<< "It's other." << std::endl;
//         return val(u8"Unknown"s);
//     }
// }

String IsBraveStr()
{
    using namespace emscripten;
    bool isBrave = !!val::global()[u8"navigator"][u8"brave"] && (val::global()[u8"navigator"][u8"brave"].call<val>(u8"isBrave").await().as<bool>());
    if (isBrave)
    {
        std::cout << "It's Brave." << std::endl;
        return u8"Brave";
    }
    else
    {
        std::cout << "It's other." << std::endl;
        return u8"Unknown";
    }
}

EMSCRIPTEN_BINDINGS(exports)
{
    USING_EMSCRIPTEN;
    EMSCRIPTEN_EXPORT_ALL_FUNCTORS(10);
    function<bool>(u8"SetVolatileValue", &Insane::Emscripten::LocalStorage::SetVolatileValue);
    function<String>(u8"GetVolatileValue", &Insane::Emscripten::LocalStorage::GetVolatileValue);
    function<void>(u8"RemoveValue", &Insane::Emscripten::LocalStorage::RemoveValue<val>);
    function<void>(u8"Clear", &Insane::Emscripten::LocalStorage::Clear);
    function<String>(u8"BrowserGetWebGLRenderer", &Insane::Emscripten::Browser::GetWebGLRenderer<String>);
    function<val>(u8"BrowserGetWebGLVendor", &Insane::Emscripten::Browser::GetWebGLVendor);
    function<val>(u8"BrowserGetName", &Insane::Emscripten::Browser::GetName<val>);
    function<val>(u8"IsBrave", &IsBrave);
    function<String>(u8"IsBraveStr", &IsBraveStr);

    register_vector<String>("VectorString");
    //function<void>(u8"RemoveVolatileValues", &Insane::Emscripten::LocalStorage::RemoveVolatileValues);
}
