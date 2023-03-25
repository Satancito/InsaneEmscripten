#include <Insane/InsaneEmscripten.h>
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#include <Insane/InsanePreprocessor.h>
#include <Insane/InsaneString.h>
#include <Insane/InsaneCryptography.h>
#include <Insane/InsaneException.h>

/* Internal API */
#define INSANE_EMSCRIPTEN_KEY "Aqui la clave"

// window.addEventListener("mousedown", event=> console.log("clicked from window's eventlistener"))
// document.addEventListener("mousedown", event=> console.log("clicked from document's eventlistener"))
// window.onmousedown = event =>{console.log("clicked from window")}
// document.onmousedown = event =>{console.log("clicked from document")}

namespace Insane::Emscripten::Internal
{

    bool TransformN(const int &base, int &result, std::string data, bool normal = true)
    {
    //     USING_NS_EMSCRIPTEN;
    //     USING_NS_INSANE_CRYPTO;
    //     data = HashManager::ToRawHmac(data, INSANE_EMSCRIPTEN_KEY, HashAlgorithm::Sha512);
    // begin:
    //     if (normal)
    //     {
    //         result = (~((~base) ^ (data[0] * data[2] * data[4] * data[16] * 612)) ^ data[data.length() - 1]) ^ (base / 216);
    //         for (char value : data)
    //         {
    //             result -= value;
    //         }
    //         return true;
    //     }
    //     else
    //     {
    //         for (char value : data)
    //         {
    //             result += value;
    //         }
    //         result = ~((~(result ^ (base / 216))) ^ data[data.length() - 1]) ^ (data[0] * data[2] * data[4] * data[16] * 612);
    //         if (result == base)
    //         {
    //             return true;
    //         }
    //         else
    //         {
    //             goto begin;
    //         }
    //     }
    //     goto begin;
    //     return false;
      
    }

    bool AreEquals(String stra, String strb)
    {
        const int b = (Insane::Cryptography::RandomManager::Next() / 612) * 216; // base
        int n = b;
        if (TransformN(b, n, stra + strb, true) && stra == strb)
        {
            TransformN(b, n, stra + strb, false);
            return true;
        }
        else
        {
            TransformN(b, ++n, stra + strb, false);
            return false;
        }
        return TransformN(b, ++n, stra + strb, false);
    }
} // namespace Insane::Emscripten::Internal

/* Operators */
EmscriptenVal Insane::Emscripten::Operator::CallOperator(const EmscriptenVal &a, const EmscriptenVal &b, const OperatorType &operatorType, const OperatorArityType &operatorArityType)
{
    // USING_NS_EMSCRIPTEN;
    // USING_NS_INSANE_CRYPTO;
    // USING_NS_INSANE_STR;
    // val global = val::global();
    // if (!VAL_INSANE)
    // {
    //     VAL_GLOBAL.set(INSANE_STRING, val::object());
    // }
    // String opName = "OP"s + HashManager::ToAlphanumericBase64(HashManager::ToBase64(RandomManager::Next(16)));
    // String script = EMPTY_STRING;
    // switch (operatorArityType)
    // {
    // case OperatorArityType::Unary:
    //     script = "Insane."s + opName + "= (a) => { return ###; };"s;
    //     break;
    // case OperatorArityType::Binary:
    //     script = "Insane."s + opName + "= (a,b) => { return ###; };"s;
    //     break;
    // default:
    //     break;
    // }

    // String op = EMPTY_STRING;
    // switch (operatorType)
    // {
    // case OperatorType::Addition:
    //     op = "a + b"s;
    //     break;
    // case OperatorType::Subtraction:
    //     op = "a - b"s;
    //     break;
    // case OperatorType::Multiplication:
    //     op = "a * b"s;
    //     break;
    // case OperatorType::Division:
    //     op = "a / b"s;
    //     break;
    // case OperatorType::Import:
    //     op = "import(a)"s;
    //     break;
    // case OperatorType::TypeOf:
    //     op = "typeof a";
    //     break;
    // case OperatorType::IsNull:
    //     op = "a === null";
    //     break;
    // case OperatorType::IsUndefined:
    //     op = "typeof a === 'undefined'";
    //     break;
    // case OperatorType::IsNullOrUndefined:
    //     break;
    // default:
    //     break;
    // }

    // global.call<val>("eval", StringExtensions::Replace(script, {{"###"s, op}}));
    // val insane = val::global()["Insane"];

    // val result = val::null();
    // switch (operatorArityType)
    // {
    // case OperatorArityType::Unary:
    //     result = insane.call<val>(opName.c_str(), a);
    //     break;
    // case OperatorArityType::Binary:
    //     result = insane.call<val>(opName.c_str(), a, b);
    //     break;
    // default:
    //     break;
    // }
    // insane.delete_(opName);
    // return result;
}

EmscriptenVal Insane::Emscripten::Operator::Add(const EmscriptenVal &a, const EmscriptenVal &b)
{
    return CallOperator(a, b, OperatorType::Addition, OperatorArityType::Binary);
}

EmscriptenVal Insane::Emscripten::Operator::Subtract(const EmscriptenVal &a, const EmscriptenVal &b)
{
    return CallOperator(a, b, OperatorType::Subtraction, OperatorArityType::Binary);
}

EmscriptenVal Insane::Emscripten::Operator::Multiply(const EmscriptenVal &a, const EmscriptenVal &b)
{
    return CallOperator(a, b, OperatorType::Multiplication, OperatorArityType::Binary);
}

EmscriptenVal Insane::Emscripten::Operator::Divide(const EmscriptenVal &a, const EmscriptenVal &b)
{
    return CallOperator(a, b, OperatorType::Division, OperatorArityType::Binary);
}

EmscriptenVal Insane::Emscripten::Operator::ImportAsync(const EmscriptenVal &a)
{
    USING_NS_EMSCRIPTEN;
    return CallOperator(a, EmscriptenVal::null(), OperatorType::Import, OperatorArityType::Unary);
}

EmscriptenVal Insane::Emscripten::Operator::TypeOf(const EmscriptenVal &a)
{
    USING_NS_EMSCRIPTEN;
    return CallOperator(a, EmscriptenVal::null(), OperatorType::TypeOf, OperatorArityType::Unary);
}

bool Insane::Emscripten::Operator::IsNull(const EmscriptenVal &a)
{
    USING_NS_EMSCRIPTEN;
    return CallOperator(a, EmscriptenVal::null(), OperatorType::IsNull, OperatorArityType::Unary).as<bool>();
}

bool Insane::Emscripten::Operator::IsUndefined(const EmscriptenVal &a)
{
    USING_NS_EMSCRIPTEN;
    return CallOperator(a, EmscriptenVal::null(), OperatorType::IsUndefined, OperatorArityType::Unary).as<bool>();
}

bool Insane::Emscripten::Operator::IsNullOrUndefined(const emscripten::val &a)
{
    return IsNull(a) || IsUndefined(a);
}

EmscriptenVal Insane::Emscripten::Operator::ToString(const emscripten::val &a)
{
    USING_NS_EMSCRIPTEN;
    return VAL_GLOBAL.call<val>("String", a);
}

/* Promise */
EmscriptenVal Insane::Emscripten::Promise::New(const EMSCRIPTEN_VOID_FUNCTOR_TYPE(2) & promiseCallback)
{
    USING_NS_EMSCRIPTEN;
    return val::global()["Promise"].new_(Js::Bind(promiseCallback));
}

/* Json */
template <>
emscripten::val Insane::Emscripten::Json::Serialize(const emscripten::val &object)
{
    USING_NS_EMSCRIPTEN;
    return val::global()["JSON"].call<val>("stringify", object);
}

template <>
String Insane::Emscripten::Json::Serialize(const emscripten::val &object)
{
    return Serialize(object).as<String>();
}

/* Converter */
// template <>
// emscripten::val Insane::Emscripten::Converter::ToString(const emscripten::val &value)
// {
//     USING_EMSCRIPTEN;
//     return val::global().call<val>("String", value);
// }

// template <>
// String Insane::Emscripten::Converter::ToString(const emscripten::val &value)
// {
//     USING_EMSCRIPTEN;
//     return ToString(value).as<String>();
// }




/* Browser */
template <>
emscripten::val Insane::Emscripten::Browser::GetUserAgent()
{
    USING_NS_EMSCRIPTEN;
    return val::global()["navigator"]["userAgent"];
}

template <>
String Insane::Emscripten::Browser::GetUserAgent<String>()
{
    return Browser::GetUserAgent().as<String>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetTimezoneOffsetMinutes()
{
    USING_NS_EMSCRIPTEN;
    return val(val::global()["Date"].new_().call<val>("getTimezoneOffset").as<int>() * -1);
}

template <>
int Insane::Emscripten::Browser::GetTimezoneOffsetMinutes()
{
    return GetTimezoneOffsetMinutes().as<int>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetTimezoneOffsetSeconds()
{
    USING_NS_EMSCRIPTEN;
    return val(GetTimezoneOffsetMinutes<int>() * 60);
}

template <>
int Insane::Emscripten::Browser::GetTimezoneOffsetSeconds()
{
    return GetTimezoneOffsetSeconds().as<int>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetTimezoneOffsetMilliseconds()
{
    USING_NS_EMSCRIPTEN;
    return val(GetTimezoneOffsetSeconds<int>() * 1000);
}

template <>
int Insane::Emscripten::Browser::GetTimezoneOffsetMilliseconds()
{
    return GetTimezoneOffsetMilliseconds().as<int>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetNameAsync(const emscripten::val &ua)
{
    USING_NS_EMSCRIPTEN;
    USING_NS_INSANE_STR;
    USING_NS_INSANE_CRYPTO;
    String userAgent = !ua ? GetUserAgent<String>() : ua.as<String>();
    if (StringExtensions::Contains(userAgent, " OPR/", false) || StringExtensions::Contains(userAgent, " Opera/", false) || !!val::global()["opr"] || !!val::global()["opera"])
    {
        return Promise::Resolve(val("Opera"));
    }

    if (Operator::TypeOf(val::global()["InstallTrigger"]).as<String>() != "undefined" || StringExtensions::Contains(userAgent, " Firefox/", false))
    {
        return Promise::Resolve(val("Firefox"));
    }

    if (StringExtensions::Contains(userAgent, " Edg/", false) || StringExtensions::Contains(userAgent, " Edge/", false))
    {
        return Promise::Resolve(val("Microsoft Edge"));
    }

    if (!!val::global()["navigator"]["brave"])
    {
        std::function<val(val)> callback = [](val isBrave) -> val
        {
            return !!isBrave ? val("Brave") : val(EMPTY_STRING);
        };
        return val::global()["navigator"]["brave"].call<val>("isBrave").call<val>("then", Js::Bind(callback));
    }

    if (StringExtensions::Contains(userAgent, " Chrome/", false) && !!val::global()["chrome"])
    {
        return Promise::Resolve(val("Google Chrome"));
    }

    if (StringExtensions::Contains(userAgent, " Version/", false) && StringExtensions::Contains(userAgent, " Safari/", false))
    {
        return Promise::Resolve(val("Safari"));
    }

    return Promise::Resolve(val(EMPTY_STRING));
}

template <>
emscripten::val Insane::Emscripten::Browser::GetNameAsync(const String &ua)
{
    USING_NS_EMSCRIPTEN;
    return GetNameAsync(ua.empty() ? GetUserAgent() : val(ua));
}

// String edgeChromium = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.121 Safari/537.36 Edg/85.0.564.63";
// String chrome = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.121 Safari/537.36";
// String firefox = "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:80.0) Gecko/20100101 Firefox/80.0";
// String vivaldi = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.119 Safari/537.36";
// String brave = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.121 Safari/537.36";
// String opera = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/84.0.4147.135 Safari/537.36 OPR/70.0.3728.189";
// String operaGX = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.102 Safari/537.36 OPR/71.0.3770.175";
// String safariMacOS = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_2) AppleWebKit/604.4.7 (KHTML, like Gecko) Version/11.0.2 Safari/604.4.7";
// String firefoxMacOS = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.12; rv:52.0) Gecko/20100101 Firefox/52.0";
// String safariMacOS2 = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_5) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/13.1.1 Safari/605.1.15";
// String safariiPhone = "Mozilla/5.0 (iPhone; CPU iPhone OS 13_1_3 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/13.0.1 Mobile/15E148 Safari/604.1";
// String chromeMacOS = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.121 Safari/537.36";
// String firefoxMacOS2 = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.13; rv:81.0) Gecko/20100101 Firefox/81.0";
// String chromeAndroid = "Mozilla/5.0 (Linux; Android 6.0; HUAWEI VNS-L23) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.101 Mobile Safari/537.36";
// String firefoxAndroid = "Mozilla/5.0 (Android 6.0; Mobile; rv:81.0) Gecko/81.0 Firefox/81.0";
// String operaMacOS = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.102 Safari/537.36 OPR/71.0.3770.171";
// String firefoxUbuntu = "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:68.0) Gecko/20100101 Firefox/68.0";
// String operaUbuntu = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.102 Safari/537.36 OPR/71.0.3770.171";
// String chromeUbuntu = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.121 Safari/537.36";
// String safariIPod = "Mozilla/5.0 (iPod; CPU iPhone OS 13_1_3 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/13.0.1 Mobile/15E148 Safari/604.1";
// String safariIPad = "Mozilla/5.0 (iPad; CPU iPhone OS 13_1_3 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/13.0.1 Mobile/15E148 Safari/604.1";
// String safariIPad ="Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/13.0.1 Safari/605.1.15";

template <>
emscripten::val Insane::Emscripten::Browser::GetOS(const emscripten::val &ua)
{

    USING_NS_EMSCRIPTEN;
    USING_NS_INSANE_STR;
    USING_NS_INSANE_CRYPTO;
    String userAgent = !ua ? GetUserAgent<String>() : ua.as<String>();
    if (StringExtensions::Contains(userAgent, "Windows"s, false) || StringExtensions::Contains(userAgent, "Win"s, false))
    {
        return val("Windows"s);
    }

    if (StringExtensions::Contains(userAgent, "Android"s, false))
    {
        return val("Android"s);
    }

    if (StringExtensions::Contains(userAgent, "Linux", false))
    {
        if (StringExtensions::Contains(userAgent, "Ubuntu", false))
        {
            return val("Ubuntu Linux"s);
        }
        return val("Linux"s);
    }

    if (StringExtensions::Contains(userAgent, "iPhone", false) ||
        StringExtensions::Contains(userAgent, "iOS", false))
    {
        if (StringExtensions::Contains(userAgent, "iPad", false))
        {
            return val("iPadOS");
        }
        if (StringExtensions::Contains(userAgent, "iPod", false))
        {
            return val("iOS iPod");
        }
        return val("iOS");
    }

    if (StringExtensions::Contains(userAgent, "Macintosh", false) ||
        StringExtensions::Contains(userAgent, "Mac", false) ||
        StringExtensions::Contains(userAgent, "Mac OS X", false) ||
        StringExtensions::Contains(userAgent, "Mac OS", false))
    {
        return val("MacOS"s);
    }

    return val(EMPTY_STRING);
}

template <>
emscripten::val Insane::Emscripten::Browser::GetOS(const String &ua)
{
    USING_NS_EMSCRIPTEN;
    return GetOS(ua.empty() ? GetUserAgent() : val(ua));
}

template <>
String Insane::Emscripten::Browser::GetOS(const emscripten::val &ua)
{
    return GetOS(ua).as<String>();
}

template <>
String Insane::Emscripten::Browser::GetOS(const String &ua)
{
    USING_NS_EMSCRIPTEN;
    return GetOS(ua.empty() ? GetUserAgent() : val(ua)).as<String>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetWebGLRenderer()
{
    USING_NS_EMSCRIPTEN;
    USING_NS_INSANE_EMSCRIPTEN;
    val webGL = val::global()["document"].call<val>("createElement", "canvas"s).call<val>("getContext", "webgl"s);
    if (!webGL)
    {
        return val(EMPTY_STRING);
    }
    val debugInfo = webGL.call<val>("getExtension", "WEBGL_debug_renderer_info"s);
    return !debugInfo ? val(EMPTY_STRING) : webGL.call<val>("getParameter", debugInfo["UNMASKED_RENDERER_WEBGL"]);
}

template <>
String Insane::Emscripten::Browser::GetWebGLRenderer<String>()
{
    USING_NS_EMSCRIPTEN;
    return GetWebGLRenderer().as<String>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetWebGLVendor()
{
    USING_NS_EMSCRIPTEN;
    USING_NS_INSANE_EMSCRIPTEN;
    val webGL = val::global()["document"].call<val>("createElement", "canvas"s).call<val>("getContext", "webgl"s);
    if (!webGL)
    {
        return val(EMPTY_STRING);
    }
    val debugInfo = webGL.call<val>("getExtension", "WEBGL_debug_renderer_info"s);
    return !debugInfo ? val(EMPTY_STRING) : webGL.call<val>("getParameter", debugInfo["UNMASKED_VENDOR_WEBGL"]);
}

template <>
String Insane::Emscripten::Browser::GetWebGLVendor<String>()
{
    USING_NS_EMSCRIPTEN;
    return GetWebGLVendor().as<String>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetAvailableScreenHeight()
{
    USING_NS_EMSCRIPTEN;
    return val::global()["screen"]["availHeight"];
}

template <>
int Insane::Emscripten::Browser::GetAvailableScreenHeight()
{
    return GetAvailableScreenHeight().as<int>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetAvailableScreenWidth()
{
    USING_NS_EMSCRIPTEN;
    return val::global()["screen"]["availWidth"];
}

template <>
int Insane::Emscripten::Browser::GetAvailableScreenWidth()
{
    return GetAvailableScreenWidth().as<int>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetAvailableScreenOrientation()
{
    USING_NS_EMSCRIPTEN;
    if (GetAvailableScreenHeight() > GetAvailableScreenWidth())
    {
        return val(static_cast<int>(OrientationType::PORTRAIT));
    }

    if (GetAvailableScreenWidth() > GetAvailableScreenHeight())
    {
        return val(static_cast<int>(OrientationType::LANDSCAPE));
    }

    return val(static_cast<int>(OrientationType::SQUARE));
}

template <>
Insane::Emscripten::OrientationType Insane::Emscripten::Browser::GetAvailableScreenOrientation()
{
    return static_cast<OrientationType>(GetAvailableScreenOrientation().as<int>());
}

template <>
emscripten::val Insane::Emscripten::Browser::GetAvailableScreenSize()
{
    USING_NS_EMSCRIPTEN;
    return val(GetAvailableScreenHeight<int>() * GetAvailableScreenWidth<int>());
}

template <>
int Insane::Emscripten::Browser::GetAvailableScreenSize()
{
    return GetAvailableScreenSize().as<int>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetScreenHeight()
{
    USING_NS_EMSCRIPTEN;
    return val::global()["screen"]["height"];
}

template <>
int Insane::Emscripten::Browser::GetScreenHeight()
{
    return GetScreenHeight().as<int>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetScreenWidth()
{
    USING_NS_EMSCRIPTEN;
    return val::global()["screen"]["width"];
}

template <>
int Insane::Emscripten::Browser::GetScreenWidth()
{
    return GetScreenWidth().as<int>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetScreenOrientation()
{
    USING_NS_EMSCRIPTEN;
    if (GetScreenHeight() > GetScreenWidth())
    {
        return val(static_cast<int>(OrientationType::PORTRAIT));
    }

    if (GetScreenWidth() > GetScreenHeight())
    {
        return val(static_cast<int>(OrientationType::LANDSCAPE));
    }

    return val(static_cast<int>(OrientationType::SQUARE));
}

template <>
Insane::Emscripten::OrientationType Insane::Emscripten::Browser::GetScreenOrientation()
{
    return static_cast<OrientationType>(GetScreenOrientation().as<int>());
}

template <>
emscripten::val Insane::Emscripten::Browser::GetScreenSize()
{
    USING_NS_EMSCRIPTEN;
    return val(GetScreenHeight<int>() * GetScreenWidth<int>());
}

template <>
int Insane::Emscripten::Browser::GetScreenSize()
{
    return GetScreenSize().as<int>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetViewportHeight()
{
    USING_NS_EMSCRIPTEN;
    return val::global()["innerHeight"];
}

template <>
int Insane::Emscripten::Browser::GetViewportHeight()
{
    return GetViewportHeight().as<int>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetViewportWidth()
{
    USING_NS_EMSCRIPTEN;
    return val::global()["innerWidth"];
}

template <>
int Insane::Emscripten::Browser::GetViewportWidth()
{
    return GetViewportWidth().as<int>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetViewportOrientation()
{
    USING_NS_EMSCRIPTEN;
    if (GetViewportHeight() > GetViewportWidth())
    {
        return val(static_cast<int>(OrientationType::PORTRAIT));
    }

    if (GetViewportWidth() > GetViewportHeight())
    {
        return val(static_cast<int>(OrientationType::LANDSCAPE));
    }

    return val(static_cast<int>(OrientationType::SQUARE));
}

template <>
Insane::Emscripten::OrientationType Insane::Emscripten::Browser::GetViewportOrientation()
{
    return static_cast<OrientationType>(GetViewportOrientation().as<int>());
}

template <>
emscripten::val Insane::Emscripten::Browser::GetViewportSize()
{
    USING_NS_EMSCRIPTEN;
    return val(GetViewportHeight<int>() * GetViewportWidth<int>());
}

template <>
int Insane::Emscripten::Browser::GetViewportSize()
{
    return GetViewportSize().as<int>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetLanguages()
{
    USING_NS_EMSCRIPTEN;
    val lang = val::global()["navigator"]["languages"];
    return !!lang ? lang : val::array();
}

template <>
std::vector<String> Insane::Emscripten::Browser::GetLanguages()
{
    USING_NS_EMSCRIPTEN;
    return vecFromJSArray<String>(GetLanguages());
}

template <>
emscripten::val Insane::Emscripten::Browser::GetLanguage()
{
    USING_NS_EMSCRIPTEN;
    return val::global()["navigator"]["language"];
}

template <>
String Insane::Emscripten::Browser::GetLanguage()
{
    USING_NS_EMSCRIPTEN;
    return GetLanguage().as<String>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetMaxTouchPoints()
{
    USING_NS_EMSCRIPTEN;
    return val::global()["navigator"]["maxTouchPoints"];
}

template <>
int Insane::Emscripten::Browser::GetMaxTouchPoints()
{
    USING_NS_EMSCRIPTEN;
    return GetMaxTouchPoints().as<int>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetDeviceMemory()
{
    USING_NS_EMSCRIPTEN;
    if (val::global()["isSecureContext"].as<bool>())
    {
        val memory = val::global()["navigator"]["deviceMemory"];
        return !!memory ? memory : val(-1);
    }
    else
    {
        return val(0);
    }
}

template <>
float Insane::Emscripten::Browser::GetDeviceMemory()
{
    return GetDeviceMemory().as<float>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetHardwareConcurrency()
{
    USING_NS_EMSCRIPTEN;
    val concurrency = val::global()["navigator"]["hardwareConcurrency"];
    return !!concurrency ? concurrency : val(-1);
}

template <>
int Insane::Emscripten::Browser::GetHardwareConcurrency()
{
    return GetHardwareConcurrency().as<int>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetPlugins()
{
    USING_NS_EMSCRIPTEN;
    val plugins = val::global()["navigator"]["plugins"];
    val result = val::array();
    if (!plugins)
    {
        return result;
    }
    val pluginsLength = plugins["length"];
    for (int i = 0; i < plugins["length"].as<int>(); i++)
    {
        val pluginValue = plugins.call<val>("item", i);
        val plugin = val::object();
        plugin.set("Name", !!pluginValue["name"] ? pluginValue["name"] : val(EMPTY_STRING));
        plugin.set("Description", !!pluginValue["description"] ? pluginValue["description"] : val(EMPTY_STRING));
        plugin.set("MimeTypes", val::array());
        val mimeTypes = val::array();
        for (int j = 0; j < pluginValue["length"].as<int>(); j++)
        {
            val mimeValue = pluginValue.call<val>("item", j);
            val mime = val::object();
            mime.set("Description", !!mimeValue["description"] ? mimeValue["description"] : val(EMPTY_STRING));
            mime.set("Type", !!mimeValue["type"] ? mimeValue["type"] : val(EMPTY_STRING));
            mime.set("Suffixes", !!mimeValue["suffixes"] ? mimeValue["suffixes"] : val(EMPTY_STRING));
            plugin["MimeTypes"].call<val>("push", mime);
        }
        result.call<val>("push", plugin);
    }
    return result;
}

template <>
std::vector<Insane::Emscripten::Browser::Plugin> Insane::Emscripten::Browser::GetPlugins()
{
    USING_NS_EMSCRIPTEN;
    std::vector<Plugin> result;
    val plugins = GetPlugins();
    for (int i = 0; i < plugins["length"].as<int>(); i++)
    {
        Plugin plugin;
        plugin.Name = plugins[i]["Name"].as<String>();
        val mimeTypes = plugins[i]["MimeTypes"];
        for (int j = 0; j < mimeTypes["length"].as<int>(); j++)
        {
            MimeType mime;
            mime.Description = mimeTypes[j]["Description"].as<String>();
            mime.Type = mimeTypes[j]["Type"].as<String>();
            mime.Suffixes = mimeTypes[j]["Suffixes"].as<String>();
            plugin.MimeTypes.push_back(mime);
        }
        result.push_back(plugin);
    }
    return result;
}

template <>
emscripten::val Insane::Emscripten::Browser::GetMimeTypes()
{
    USING_NS_EMSCRIPTEN;
    val mimeTypes = val::global()["navigator"]["mimeTypes"];
    val result = val::global().array();
    if (!mimeTypes)
    {
        return result;
    }
    for (int i = 0; i < mimeTypes["length"].as<int>(); i++)
    {
        val mimeValue = mimeTypes.call<val>("item", i);
        val mime = val::object();
        mime.set("Description", !!mimeValue["description"] ? mimeValue["description"] : val(EMPTY_STRING));
        mime.set("Type", !!mimeValue["type"] ? mimeValue["type"] : val(EMPTY_STRING));
        mime.set("Suffixes", !!mimeValue["suffixes"] ? mimeValue["suffixes"] : val(EMPTY_STRING));
        result.call<val>("push", mime);
    }
    return result;
}

template <>
std::vector<Insane::Emscripten::Browser::MimeType> Insane::Emscripten::Browser::GetMimeTypes()
{
    USING_NS_EMSCRIPTEN;
    std::vector<MimeType> result;
    val mimes = GetMimeTypes();
    for (int i = 0; i < mimes["length"].as<int>(); i++)
    {
        MimeType mime;
        mime.Description = mimes[i]["Description"].as<String>();
        mime.Type = mimes[i]["Type"].as<String>();
        mime.Suffixes = mimes[i]["Suffixes"].as<String>();
        result.push_back(mime);
    }
    return result;
}

template <>
emscripten::val Insane::Emscripten::Browser::GetDoNotTrack()
{
    USING_NS_EMSCRIPTEN;
    val dnt = val::global()["navigator"]["doNotTrack"];
    return !!dnt ? dnt : val("unspecified"s);
}

template <>
String Insane::Emscripten::Browser::GetDoNotTrack()
{
    return GetDoNotTrack().as<String>();
}

template <>
emscripten::val Insane::Emscripten::Browser::HasCookiesSupport()
{
    USING_NS_EMSCRIPTEN;
    USING_NS_INSANE_STR;
    bool result = true && !!val::global()["navigator"]["cookieEnabled"];
    if (!!result)
    {
        val::global()["document"].set("cookie", "InsaneCookie=1"_val);
        result = StringExtensions::Contains(val::global()["document"]["cookie"].as<String>(), "InsaneCookie=");
        val::global()["document"].set("cookie", "InsaneCookie=1;expires="s + val::global()["Date"].new_(1970, 1, 1, 0, 0, 0, 0).call<val>("toGMTString").as<String>());
    }
    return val(result);
}

template <>
bool Insane::Emscripten::Browser::HasCookiesSupport()
{
    return HasCookiesSupport().as<bool>();
}

template <>
EmscriptenVal Insane::Emscripten::Browser::GetFingerprintAsync(const EmscriptenVal &key)
{
    USING_NS_EMSCRIPTEN;
    USING_NS_INSANE_CRYPTO;
    String result = Browser::GetUserAgent<String>();
    result += std::to_string(Browser::GetTimezoneOffsetMilliseconds<int>());
    result += std::to_string(Browser::GetTimezoneOffsetSeconds<int>());
    result += std::to_string(Browser::GetTimezoneOffsetMinutes<int>());

    EMSCRIPTEN_VAL_FUNCTOR_TYPE(1)
    callback = [result, key](emscripten::val name) -> emscripten::val
    {
        String ret = result + name.as<String>();
        ret += Browser::GetOS<String>();
        ret += Browser::GetWebGLRenderer<String>();
        ret += Browser::GetWebGLVendor<String>();
        ret += std::to_string(Browser::GetScreenSize<int>());
        ret += Browser::GetLanguage<String>();
        ret += Json::Serialize<String>(Browser::GetLanguages());
        ret += std::to_string(Browser::GetMaxTouchPoints<int>());
        ret += std::to_string(Browser::GetDeviceMemory<float>());
        ret += std::to_string(Browser::GetHardwareConcurrency<int>());
        ret += Json::Serialize<String>(Browser::GetMimeTypes());
        ret += Json::Serialize<String>(Browser::GetPlugins());
        ret += Browser::GetDoNotTrack<String>();
        ret += std::to_string(Browser::HasCookiesSupport<bool>());
        val objectClass = val::global()["Object"];
        val props = objectClass.call<val>("getOwnPropertyNames", objectClass.call<val>("getPrototypeOf", val::global()["navigator"])).call<val>("sort");
        ret += Json::Serialize<String>(props);
        ret += Operator::ToString(props["length"]).as<String>();
        //return val("Insane"s + HashManager::ToBase64Hmac(ret, key.as<String>()).Hash());
    };
    return Browser::GetNameAsync().call<val>("then", Js::Bind(callback));
}

template <>
EmscriptenVal Insane::Emscripten::Browser::GetFingerprintAsync(const String &key)
{
    USING_NS_EMSCRIPTEN;
    return GetFingerprintAsync(val(key));
}

/* Js */

void Insane::Emscripten::Js::SetProperty(EmscriptenVal object, const String &property, const EmscriptenVal &value, const bool &replaceIfExists)
{
    if (Operator::IsNullOrUndefined(object[property]))
    {
        object.set(property, value);
    }
    else
    {
        if (replaceIfExists)
        {
            object.set(property, value);
        }
    }
}

void Insane::Emscripten::Js::SetPropertyObject(EmscriptenVal object, const String &property, const bool &replaceIfExists)
{
    SetProperty(object, property, EmscriptenVal::global().object(), replaceIfExists);
}

void Insane::Emscripten::Js::SetPropertyArray(EmscriptenVal object, const String &property, const bool &replaceIfExists)
{
    SetProperty(object, property, EmscriptenVal::global().array(), replaceIfExists);
}

void Insane::Emscripten::Js::SetPropertyNull(EmscriptenVal object, const String &property, const bool &replaceIfExists)
{
    SetProperty(object, property, EmscriptenVal::global().null(), replaceIfExists);
}

String Insane::Emscripten::Js::GetPropertyName(const String &name, const String &key, const String &suffix)
{
    // USING_NS_INSANE_CRYPTO;
    // USING_NS_INSANE_STR;
    // return (suffix.empty() ? INSANE_PROPERTY_SUFFIX : suffix) + HashManager::ToAlphanumericBase64(HashManager::ToRawHmac(name, INSANE_EMSCRIPTEN_KEY + key, HashAlgorithm::Sha512));
}

emscripten::val Insane::Emscripten::Js::Bind(const emscripten::val &fx)
{
    USING_NS_EMSCRIPTEN;
    return fx["opcall"].call<val>("bind", fx);
}

template <>
emscripten::val Insane::Emscripten::Js::LoadScriptAsync(const emscripten::val &scriptpath)
{
    USING_NS_EMSCRIPTEN;
    String id = Js::GetPropertyName(scriptpath.as<String>(), EMPTY_STRING, INSANE_PROPERTY_SUFFIX);
    Js::SetPropertyObject(val::global(), INSANE_STRING, false);
    String loadedName = Js::GetPropertyName("Loaded"s, EMPTY_STRING, INSANE_PROPERTY_SUFFIX);
    Js::SetPropertyObject(val::global()["Insane"], loadedName, false);
    val loaded = VAL_INSANE[loadedName];
    if (!Operator::IsNullOrUndefined(loaded[id]))
    {
        return Promise::Resolve(id);
    }
    loaded.set(id, val(true));
    EMSCRIPTEN_VOID_FUNCTOR_TYPE(2)
    promiseCallback = [scriptpath, id, loaded](val resolve, val reject) -> void
    {
        val script = val::global("document").call<val>("createElement", val("script"));
        script.set("id", val(id));
        script.set("src", scriptpath);
        script.set("type", val("text/javascript"));
        EMSCRIPTEN_VOID_FUNCTOR_TYPE(1)
        onloadCallback = [scriptpath, resolve, id](val a) -> void
        {
            resolve(id);
        };
        script.set("onload", Js::Bind(onloadCallback));
        EMSCRIPTEN_VOID_FUNCTOR_TYPE(1)
        onerrorCallback = [scriptpath, id, reject, loaded](val a) -> void
        {
            val::global("document").call<val>("getElementById", val(id)).call<void>("remove");
            loaded.delete_(id);
            reject(id);
        };
        script.set("onerror", Js::Bind(onerrorCallback));
        val::global("document")["body"].call<val>("appendChild", script);
    };
    return Promise::New(promiseCallback);
}

template <>
emscripten::val Insane::Emscripten::Js::LoadScriptAsync(const String &scriptpath)
{
    USING_NS_EMSCRIPTEN;
    return LoadScriptAsync(val(scriptpath));
}

/* LocalStorage */

template <>
emscripten::val Insane::Emscripten::LocalStorage::GetValue(const EmscriptenVal &key, const EmscriptenVal &password) noexcept
{
    USING_NS_EMSCRIPTEN;
    USING_NS_INSANE_EXCEPTION;
    USING_NS_INSANE_CRYPTO;
    USING_NS_INSANE_STR;
    try
    {
        String passwordStr = Operator::IsNullOrUndefined(password) ? EMPTY_STRING : Operator::ToString(password).as<String>();
        const val value = val::global("localStorage").call<val>("getItem", key);
        if (passwordStr == EMPTY_STRING)
        {
            return !value ? val(EMPTY_STRING) : value;
        }
        return !value ? val(EMPTY_STRING) : val(AesManager::DecryptFromBase64(value.as<String>(), passwordStr));
    }
    catch (const std::exception &ex)
    {
        return val(EMPTY_STRING);
    }
    catch (...)
    {
        return val(EMPTY_STRING);
    }
}

template <>
String Insane::Emscripten::LocalStorage::GetValue(const EmscriptenVal &key, const EmscriptenVal &password) noexcept
{
    return GetValue(key, password).as<String>();
}

template <>
EmscriptenVal Insane::Emscripten::LocalStorage::GetValue(const String &key, const String &password) noexcept
{
    USING_NS_EMSCRIPTEN;
    return GetValue(val(key), val(password));
}

template <>
String Insane::Emscripten::LocalStorage::GetValue(const String &key, const String &password) noexcept
{
    USING_NS_EMSCRIPTEN;
    return GetValue(val(key), val(password)).as<String>();
}

template <>
void Insane::Emscripten::LocalStorage::SetValue(const EmscriptenVal &key, const EmscriptenVal &value, const EmscriptenVal &password) noexcept
{
    USING_NS_EMSCRIPTEN;
    USING_NS_INSANE_EXCEPTION;
    USING_NS_INSANE_CRYPTO;
    USING_NS_INSANE_STR;
    try
    {
        String passwordStr = Operator::IsNullOrUndefined(password) ? EMPTY_STRING : Operator::ToString(password).as<String>();
        if (passwordStr == EMPTY_STRING)
        {
            val::global("localStorage").call<void>("setItem", key, value);
            return;
        }
        val::global("localStorage").call<void>("setItem", key, AesManager::EncryptToBase64(Operator::ToString(value).as<String>(), passwordStr));
    }
    catch (const ExceptionBase &e)
    {
    }
}

template <>
void Insane::Emscripten::LocalStorage::SetValue(const String &key, const String &value, const String &password) noexcept
{
    USING_NS_EMSCRIPTEN;
    SetValue(val(key), val(value), val(password));
}

void Insane::Emscripten::Js::ThrowError(const String &message)
{
    USING_NS_EMSCRIPTEN;
    USING_NS_INSANE_EXCEPTION;
    val::global("Error").new_(val(message)).throw_();
}

template <>
void Insane::Emscripten::LocalStorage::RemoveValue(const emscripten::val &key) noexcept
{
    USING_NS_EMSCRIPTEN;
    val::global("localStorage").call<void>("removeItem", key);
}

template <>
void Insane::Emscripten::LocalStorage::RemoveValue(const String &key) noexcept
{
    USING_NS_EMSCRIPTEN;
    RemoveValue(val(key));
}

void Insane::Emscripten::LocalStorage::Clear()
{
    USING_NS_EMSCRIPTEN;
    val::global("localStorage").call<void>("clear");
}

template <>
void Insane::Emscripten::LocalStorage::RemoveValuesStartingWith(const EmscriptenVal &preffix)
{
    USING_NS_EMSCRIPTEN;
    USING_NS_INSANE_STR;
    for (val value : emscripten::vecFromJSArray<val>(val::global("Object").call<val>("entries", val::global("localStorage"))))
    {
        if (StringExtensions::StartsWith(value[0].as<String>(), preffix.as<String>()))
        {
            LocalStorage::RemoveValue(value[0]);
        }
    }
}

template <>
void Insane::Emscripten::LocalStorage::RemoveValuesStartingWith(const String &preffix)
{
    USING_NS_EMSCRIPTEN;
    RemoveValuesStartingWith(val(preffix));
}

/* No eliminar,
esta función debe ir en el archivo .h, se encuentra en pruebas la implementación de especializaciones,
si no cumple las espectativas se debe borrar el código y dejar solamente esta función*/
// template <typename ReturnType, typename... ParametersType>
// static inline emscripten::val Bind(const std::function<ReturnType(ParametersType...)> &fx);
// {
//     USING_EMSCRIPTEN;
//     return val(fx)["opcall"].call<val>("bind", val(fx));
// }