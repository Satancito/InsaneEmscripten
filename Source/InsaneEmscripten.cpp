#include <Insane/InsaneCryptography.h>
#include <Insane/InsaneEmscripten.h>
#include <Insane/InsaneException.h>
#include <Insane/InsanePreprocessor.h>
#include <Insane/InsaneString.h>
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>
#include <utility>


USING_NS_EMSCRIPTEN;
USING_NS_INSANE_EXCEPTION;
USING_NS_INSANE_CRYPTO;
USING_NS_INSANE_STR;
USING_NS_INSANE_CORE;
USING_NS_INSANE_EMSCRIPTEN;

// namespace Internal
// {
//     bool TransformN(const int &base, int &result, std::string data, const String &key, bool normal = true)
//     {
//         data = HashExtensions::ToHmac(data, key, HashAlgorithm::Sha512);
//     begin:
//         if (normal)
//         {
//             result = (~((~base) ^ (data[0] * data[2] * data[4] * data[16] * 612)) ^ data[data.length() - 1]) ^ (base / 216);
//             for (char value : data)
//             {
//                 result -= value;
//             }
//             return true;
//         }
//         else
//         {
//             for (char value : data)
//             {
//                 result += value;
//             }
//             result = ~((~(result ^ (base / 216))) ^ data[data.length() - 1]) ^ (data[0] * data[2] * data[4] * data[16] * 612);
//             if (result == base)
//             {
//                 return true;
//             }
//             else
//             {
//                 goto begin;
//             }
//         }
//         goto begin;
//         return false;
//     }

//     bool AreEquals(String stra, String strb, const String &key)
//     {
//         USING_NS_INSANE_CRYPTO;
//         const int b = (RandomExtensions::Next() / 612) * 216; // base
//         int n = b;
//         if (TransformN(b, n, stra + strb, key, true) && stra == strb)
//         {
//             TransformN(b, n, stra + strb, key, false);
//             return true;
//         }
//         else
//         {
//             TransformN(b, ++n, stra + strb, key, false);
//             return false;
//         }
//         return TransformN(b, ++n, stra + strb, key, false);
//     }
// } // namespace Internal

/* Operators */
EmscriptenVal JsOperator::CallOperator(const EmscriptenVal &a, const EmscriptenVal &b, const OperatorType &operatorType, const OperatorArityType &operatorArityType)
{
    val global = val::global();
    if (!EMVAL_INSANE)
    {
        EMVAL_GLOBAL.set(INSANE_STRING, val::object());
    }
    String opName = "OP"s + HexEncodingExtensions::EncodeToHex(RandomExtensions::NextBytes(16));
    String script = EMPTY_STRING;
    switch (operatorArityType)
    {
        case OperatorArityType::Unary:
            script = "Insane."s + opName + "= (a) => { return ###; };"s;
            break;
        case OperatorArityType::Binary:
            script = "Insane."s + opName + "= (a,b) => { return ###; };"s;
            break;
        default:
            break;
    }

    String op = EMPTY_STRING;
    switch (operatorType)
    {
        case OperatorType::Addition:
            op = "a + b"s;
            break;
        case OperatorType::Subtraction:
            op = "a - b"s;
            break;
        case OperatorType::Multiplication:
            op = "a * b"s;
            break;
        case OperatorType::Division:
            op = "a / b"s;
            break;
        case OperatorType::Import:
            op = "import(a)"s;
            break;
        case OperatorType::TypeOf:
            op = "typeof a";
            break;
        case OperatorType::IsNull:
            op = "a === null";
            break;
        case OperatorType::IsUndefined:
            op = "typeof a === 'undefined'";
            break;
        case OperatorType::IsNullOrUndefined:
            break;
        default:
            break;
    }

    global.call<val>("eval", StringExtensions::Replace(script, {{"###"s, op}}));
    val insane = val::global()["Insane"];

    val result = val::null();
    switch (operatorArityType)
    {
        case OperatorArityType::Unary:
            result = insane.call<val>(opName.c_str(), a);
            break;
        case OperatorArityType::Binary:
            result = insane.call<val>(opName.c_str(), a, b);
            break;
        default:
            break;
    }
    insane.delete_(opName);
    return result;
}

EmscriptenVal JsOperator::Add(const EmscriptenVal &a, const EmscriptenVal &b)
{
    return CallOperator(a, b, OperatorType::Addition, OperatorArityType::Binary);
}

EmscriptenVal JsOperator::Subtract(const EmscriptenVal &a, const EmscriptenVal &b)
{
    return CallOperator(a, b, OperatorType::Subtraction, OperatorArityType::Binary);
}

EmscriptenVal JsOperator::Multiply(const EmscriptenVal &a, const EmscriptenVal &b)
{
    return CallOperator(a, b, OperatorType::Multiplication, OperatorArityType::Binary);
}

EmscriptenVal JsOperator::Divide(const EmscriptenVal &a, const EmscriptenVal &b)
{
    return CallOperator(a, b, OperatorType::Division, OperatorArityType::Binary);
}

EmscriptenVal JsOperator::ImportAsync(const EmscriptenVal &a)
{
    return CallOperator(a, EmscriptenVal::null(), OperatorType::Import, OperatorArityType::Unary);
}

EmscriptenVal JsOperator::TypeOf(const EmscriptenVal &a)
{
    return CallOperator(a, EmscriptenVal::null(), OperatorType::TypeOf, OperatorArityType::Unary);
}

bool JsOperator::IsNull(const EmscriptenVal &a)
{
    return CallOperator(a, EmscriptenVal::null(), OperatorType::IsNull, OperatorArityType::Unary).as<bool>();
}

bool JsOperator::IsUndefined(const EmscriptenVal &a)
{
    return CallOperator(a, EmscriptenVal::null(), OperatorType::IsUndefined, OperatorArityType::Unary).as<bool>();
}

bool JsOperator::IsNullOrUndefined(const emscripten::val &a)
{
    return IsNull(a) || IsUndefined(a);
}

EmscriptenVal JsOperator::ToString(const emscripten::val &a)
{
    return EMVAL_GLOBAL.call<val>("String", a);
}

/* Promise */
EmscriptenVal Promise::New(const EMSCRIPTEN_VOID_FUNCTOR_TYPE(2) & promiseCallback)
{
    return val::global()["Promise"].new_(Js::Bind(promiseCallback));
}

/* Json */
template <>
emscripten::val Json::Serialize(const emscripten::val &object)
{
    return val::global()["JSON"].call<val>("stringify", object);
}

template <>
String Json::Serialize(const emscripten::val &object)
{
    return Serialize(object).as<String>();
}

/* Converter */
// template <>
// emscripten::val Converter::ToString(const emscripten::val &value)
// {
//     USING_EMSCRIPTEN;
//     return val::global().call<val>("String", value);
// }

// template <>
// String Converter::ToString(const emscripten::val &value)
// {
//     USING_EMSCRIPTEN;
//     return ToString(value).as<String>();
// }

/* Browser */
template <>
emscripten::val Browser::GetUserAgent()
{
    return val::global()["navigator"]["userAgent"];
}

template <>
String Browser::GetUserAgent<String>()
{
    return Browser::GetUserAgent().as<String>();
}

template <>
emscripten::val Browser::GetTimezoneOffsetMinutes()
{
    return val(val::global()["Date"].new_().call<val>("getTimezoneOffset").as<int>() * -1);
}

template <>
int Browser::GetTimezoneOffsetMinutes()
{
    return GetTimezoneOffsetMinutes().as<int>();
}

template <>
emscripten::val Browser::GetTimezoneOffsetSeconds()
{
    return val(GetTimezoneOffsetMinutes<int>() * 60);
}

template <>
int Browser::GetTimezoneOffsetSeconds()
{
    return GetTimezoneOffsetSeconds().as<int>();
}

template <>
emscripten::val Browser::GetTimezoneOffsetMilliseconds()
{
    USING_NS_EMSCRIPTEN;
    return val(GetTimezoneOffsetSeconds<int>() * 1000);
}

template <>
int Browser::GetTimezoneOffsetMilliseconds()
{
    return GetTimezoneOffsetMilliseconds().as<int>();
}

template <>
emscripten::val Browser::GetNameAsync(const emscripten::val &ua)
{
    String userAgent = !ua ? GetUserAgent<String>() : ua.as<String>();
    if (StringExtensions::Contains(userAgent, " OPR/", false) || StringExtensions::Contains(userAgent, " Opera/", false) || !!val::global()["opr"] || !!val::global()["opera"])
    {
        return Promise::Resolve(val("Opera"));
    }

    if (JsOperator::TypeOf(val::global()["InstallTrigger"]).as<String>() != "undefined" || StringExtensions::Contains(userAgent, " Firefox/", false))
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
emscripten::val Browser::GetNameAsync(const String &ua)
{
    return GetNameAsync(ua.empty() ? GetUserAgent() : val(ua));
}

template <>
emscripten::val Browser::GetOS(const emscripten::val &ua)
{
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
emscripten::val Browser::GetOS(const String &ua)
{
    return GetOS(ua.empty() ? GetUserAgent() : val(ua));
}

template <>
String Browser::GetOS(const emscripten::val &ua)
{
    return GetOS(ua).as<String>();
}

template <>
String Browser::GetOS(const String &ua)
{
    return GetOS(ua.empty() ? GetUserAgent() : val(ua)).as<String>();
}

template <>
emscripten::val Browser::GetWebGLRenderer()
{
    val canvas = val::global()["document"].call<val>("createElement", "canvas"s);
    val context = canvas.call<val>("getContext", "webgl"s);
    if (!context)
    {
        canvas.call<void>("remove");
        return val(EMPTY_STRING);
    }
    val webGlRenderInfo = context.call<val>("getExtension", "WEBGL_debug_renderer_info"s);
    val result = val::null();
    if (!webGlRenderInfo)
    {
        result = context.call<val>("getParameter", context.call<val>("RENDERER"));
    }
    else
    {
        result = context.call<val>("getParameter", webGlRenderInfo["UNMASKED_RENDERER_WEBGL"]);
    }
    val extension = context.call<val>("getExtension", "WEBGL_lose_context"s);
    if (!!extension)
    {
        extension.call<void>("loseContext");
        context = val::null();
    }
    canvas.call<void>("remove");
    return !result ? val(EMPTY_STRING) : result;
}

template <>
String Browser::GetWebGLRenderer<String>()
{
    return GetWebGLRenderer().as<String>();
}

template <>
emscripten::val Browser::GetWebGLVendor()
{
    val canvas = val::global()["document"].call<val>("createElement", "canvas"s);
    val context = canvas.call<val>("getContext", "webgl"s);
    if (!context)
    {
        canvas.call<void>("remove");
        return val(EMPTY_STRING);
    }
    val webGlRenderInfo = context.call<val>("getExtension", "WEBGL_debug_renderer_info"s);
    val result = val::null();
    if (!webGlRenderInfo)
    {
        result = context.call<val>("getParameter", context.call<val>("RENDERER"));
    }
    else
    {
        result = context.call<val>("getParameter", webGlRenderInfo["UNMASKED_VENDOR_WEBGL"]);
    }
    val extension = context.call<val>("getExtension", "WEBGL_lose_context"s);
    if (!!extension)
    {
        extension.call<void>("loseContext");
        context = val::null();
    }
    canvas.call<void>("remove");
    return !result ? val(EMPTY_STRING) : result;
}

template <>
String Browser::GetWebGLVendor<String>()
{
    return GetWebGLVendor().as<String>();
}

template <>
emscripten::val Browser::GetAvailableScreenHeight()
{
    return val::global()["screen"]["availHeight"];
}

template <>
int Browser::GetAvailableScreenHeight()
{
    return GetAvailableScreenHeight().as<int>();
}

template <>
emscripten::val Browser::GetAvailableScreenWidth()
{
    return val::global()["screen"]["availWidth"];
}

template <>
int Browser::GetAvailableScreenWidth()
{
    return GetAvailableScreenWidth().as<int>();
}

template <>
emscripten::val Browser::GetAvailableScreenOrientation()
{
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
OrientationType Browser::GetAvailableScreenOrientation()
{
    return static_cast<OrientationType>(GetAvailableScreenOrientation().as<int>());
}

template <>
emscripten::val Browser::GetAvailableScreenSize()
{
    return val(GetAvailableScreenHeight<int>() * GetAvailableScreenWidth<int>());
}

template <>
int Browser::GetAvailableScreenSize()
{
    return GetAvailableScreenSize().as<int>();
}

template <>
emscripten::val Browser::GetScreenHeight()
{
    return val::global()["screen"]["height"];
}

template <>
int Browser::GetScreenHeight()
{
    return GetScreenHeight().as<int>();
}

template <>
emscripten::val Browser::GetScreenWidth()
{
    return val::global()["screen"]["width"];
}

template <>
int Browser::GetScreenWidth()
{
    return GetScreenWidth().as<int>();
}

template <>
emscripten::val Browser::GetScreenOrientation()
{
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
OrientationType Browser::GetScreenOrientation()
{
    return static_cast<OrientationType>(GetScreenOrientation().as<int>());
}

template <>
emscripten::val Browser::GetScreenSize()
{
    USING_NS_EMSCRIPTEN;
    return val(GetScreenHeight<int>() * GetScreenWidth<int>());
}

template <>
int Browser::GetScreenSize()
{
    return GetScreenSize().as<int>();
}

template <>
emscripten::val Browser::GetViewportHeight()
{
    USING_NS_EMSCRIPTEN;
    return val::global()["innerHeight"];
}

template <>
int Browser::GetViewportHeight()
{
    return GetViewportHeight().as<int>();
}

template <>
emscripten::val Browser::GetViewportWidth()
{
    return val::global()["innerWidth"];
}

template <>
int Browser::GetViewportWidth()
{
    return GetViewportWidth().as<int>();
}

template <>
emscripten::val Browser::GetViewportOrientation()
{
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
OrientationType Browser::GetViewportOrientation()
{
    return static_cast<OrientationType>(GetViewportOrientation().as<int>());
}

template <>
emscripten::val Browser::GetViewportSize()
{
    return val(GetViewportHeight<int>() * GetViewportWidth<int>());
}

template <>
int Browser::GetViewportSize()
{
    return GetViewportSize().as<int>();
}

template <>
emscripten::val Browser::GetLanguages()
{
    val lang = val::global()["navigator"]["languages"];
    return !!lang ? lang : val::array();
}

template <>
std::vector<String> Browser::GetLanguages()
{
    return vecFromJSArray<String>(GetLanguages());
}

template <>
emscripten::val Browser::GetLanguage()
{
    return val::global()["navigator"]["language"];
}

template <>
String Browser::GetLanguage()
{
    return GetLanguage().as<String>();
}

template <>
emscripten::val Browser::GetMaxTouchPoints()
{
    return val::global()["navigator"]["maxTouchPoints"];
}

template <>
int Browser::GetMaxTouchPoints()
{
    return GetMaxTouchPoints().as<int>();
}

template <>
emscripten::val Browser::GetDeviceMemory()
{
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
float Browser::GetDeviceMemory()
{
    return GetDeviceMemory().as<float>();
}

template <>
emscripten::val Browser::GetHardwareConcurrency()
{
    val concurrency = val::global()["navigator"]["hardwareConcurrency"];
    return !!concurrency ? concurrency : val(-1);
}

template <>
int Browser::GetHardwareConcurrency()
{
    return GetHardwareConcurrency().as<int>();
}

template <>
emscripten::val Browser::GetPlugins()
{
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
std::vector<Browser::Plugin> Browser::GetPlugins()
{
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
emscripten::val Browser::GetMimeTypes()
{
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
std::vector<Browser::MimeType> Browser::GetMimeTypes()
{
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
emscripten::val Browser::GetDoNotTrack()
{
    val dnt = val::global()["navigator"]["doNotTrack"];
    return !!dnt ? dnt : val("unspecified"s);
}

template <>
String Browser::GetDoNotTrack()
{
    return GetDoNotTrack().as<String>();
}

template <>
emscripten::val Browser::HasCookiesSupport()
{
    bool result = true && !!val::global()["navigator"]["cookieEnabled"];
    if (!!result)
    {
        val::global()["document"].set("cookie", "InsaneCookie=1;SameSite=Strict"_val);
        result = StringExtensions::Contains(val::global()["document"]["cookie"].as<String>(), "InsaneCookie=0;SameSite=Strict;");
        val::global()["document"].set("cookie", "InsaneCookie=1;SameSite=Strict;expires="s + val::global()["Date"].new_(1970, 1, 1, 0, 0, 0, 0).call<val>("toGMTString").as<String>());
    }
    return val(result);
}

template <>
bool Browser::HasCookiesSupport()
{
    return HasCookiesSupport().as<bool>();
}

template <>
EmscriptenVal Browser::GetFingerprintAsync(const EmscriptenVal &key)
{
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
        ret += JsOperator::ToString(props["length"]).as<String>();
        return val("Insane"s + Base64EncodingExtensions::EncodeToBase64(HashExtensions::ComputeHmac(ConverterExtensions::StringToStdVectorUint8(ret), ConverterExtensions::StringToStdVectorUint8(key.as<String>()))));
    };
    return Browser::GetNameAsync().call<val>("then", Js::Bind(callback));
}

template <>
EmscriptenVal Browser::GetFingerprintAsync(const String &key)
{
    return GetFingerprintAsync(val(key));
}

/* Js */
Emval Js::SetProperty(Emval &object, const String &property, const Emval &value, const bool &replaceIfExists)
{
    Emval result = object;
    if (JsOperator::IsNullOrUndefined(result[property]))
    {
        result.set(property, value);
    }
    else
    {
        if (replaceIfExists)
        {
            result.set(property, value);
        }
    }
    return result;
}

Emval Js::SetPropertyEmptyObject(EmscriptenVal &object, const String &property, const bool &replaceIfExists)
{
    return SetProperty(object, property, EmscriptenVal::global().object(), replaceIfExists);
}

Emval Js::SetPropertyEmptyArray(EmscriptenVal &object, const String &property, const bool &replaceIfExists)
{
    return SetProperty(object, property, EmscriptenVal::global().array(), replaceIfExists);
}

Emval Js::SetPropertyNull(EmscriptenVal &object, const String &property, const bool &replaceIfExists)
{
    return SetProperty(object, property, EmscriptenVal::global().null(), replaceIfExists);
}

String Js::GetPropertyName(const String &name, const StdUniquePtr<Cryptography::IHasher> &keyHasher, const String &keyPrefix)
{
    return (keyPrefix.empty() ? INSANE_STRING : keyPrefix) + "_" + (keyHasher != nullptr ? keyHasher->ComputeEncoded(name) : name);
}

template <>
emscripten::val Js::LoadScriptAsync(const emscripten::val &scriptpath)
{
    String id = Js::GetPropertyName(scriptpath.as<String>());
    Emval object = val::global();
    Js::SetPropertyEmptyObject(object, INSANE_STRING, false);
    String loadedName = Js::GetPropertyName("Loaded"s);
    Js::SetPropertyEmptyObject(object = object["Insane"], loadedName, false);
    val loaded = EMVAL_INSANE[loadedName];
    if (!JsOperator::IsNullOrUndefined(loaded[id]))
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
        onloadCallback = [scriptpath, resolve, id]([[maybe_unused]] val a) -> void
        {
            resolve(id);
        };
        script.set("onload", Js::Bind(onloadCallback));
        EMSCRIPTEN_VOID_FUNCTOR_TYPE(1)
        onerrorCallback = [scriptpath, id, reject, loaded]([[maybe_unused]] val a) -> void
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
emscripten::val Js::LoadScriptAsync(const String &scriptpath)
{
    return LoadScriptAsync(val(scriptpath));
}

Emval Js::NewError(const String &message, const Emval &options)
{
    return EMVAL_GLOBAL["Error"].new_(message, options);
}

void Js::ThrowError(const String &message, const Emval &options)
{
    NewError(message, options).throw_();
}

/* LocalStorage */
void LocalStorage::SetItem(const String &key, const String &value, const StdUniquePtr<Cryptography::IEncryptor> &valueEncryptor, const StdUniquePtr<Cryptography::IHasher> &keyHasher, const String &keyPrefix)
{
    try
    {
        EMVAL_GLOBAL["localStorage"].call<EmscriptenVal>("setItem", Js::GetPropertyName(key, keyHasher, keyPrefix), valueEncryptor != nullptr ? valueEncryptor->EncryptEncoded(value) : value);
    }
    catch (...)
    {
        __INSANE_THROW_DEFAULT_EXCEPTION(JsException, DebugType::Debug);
    }
}

EmscriptenVal LocalStorage::GetItem(const String &key, const StdUniquePtr<Cryptography::IEncryptor> &valueEncryptor, const StdUniquePtr<Cryptography::IHasher> &keyHasher, const String &keyPrefix)
{
    try
    {
        EmscriptenVal value = EMVAL_GLOBAL["localStorage"].call<EmscriptenVal>("getItem", Js::GetPropertyName(key, keyHasher, keyPrefix));
        if (!value)
        {
            return value;
        }
        return valueEncryptor != nullptr ? EmscriptenVal(ConverterExtensions::StdVectorUint8ToString(valueEncryptor->DecryptEncoded(value.as<String>()))) : value;
    }
    catch (...)
    {
        __INSANE_THROW_DEFAULT_EXCEPTION(JsException, DebugType::Debug);
    }
}

void LocalStorage::RemoveItem(const String &key, const StdUniquePtr<Cryptography::IHasher> &keyHasher, const String &keyPrefix)
{
    EMVAL_GLOBAL["localStorage"].call<EmscriptenVal>("removeItem", Js::GetPropertyName(key, keyHasher, keyPrefix));
}

void LocalStorage::Clear()
{
    EMVAL_GLOBAL["localStorage"].call<EmscriptenVal>("clear");
}

void LocalStorage::RemoveItemsWithKeyPrefix(const String &prefix)
{
    StdVector<EmscriptenVal> items;
    int localStorageSize = EMVAL_GLOBAL["localStorage"]["length"].as<int>();
    for (int i = 0; i < localStorageSize; i++)
    {
        EmscriptenVal key = EMVAL_GLOBAL["localStorage"].call<EmscriptenVal>("key", EmscriptenVal(i));
        if (StringExtensions::StartsWith(key.as<String>(), prefix))
        {
            items.push_back(key);
        }
    }
    for (EmscriptenVal value : items)
    {
        RemoveItem(value.as<String>());
    }
}

// ███ Interop ███
StdVectorUint8 InteropExtensions::JsUint8ArrayToStdVectorUint8(const EmscriptenVal &uint8Array)
{
    size_t length = uint8Array["length"].as<size_t>();
    std::vector<uint8_t> result;
    for (size_t i = 0; i < length; i++)
    {
        result.push_back(uint8Array[i].as<uint8_t>());
    }
    return result;
}

StdVectorUint8 InteropExtensions::JsStringToStdVectorUint8(const EmscriptenVal &str)
{
    return ConverterExtensions::StringToStdVectorUint8(str.as<String>());
}

EmscriptenVal InteropExtensions::StdVectorUint8ToJsUint8Array(const StdVectorUint8 &vector)
{
    val array8 = val::array(vector);
    return val::global("Uint8Array").new_(array8);
}

void InteropExtensions::PrintStdVectorUint8(StdVectorUint8 vector)
{
    for (size_t i = 0; i < vector.size(); i++)
    {
        Console::Log(vector[i]);
    }
}

// ███ FetchOptions ███

FetchOptions::FetchOptions(const FetchOptions &options)
    : _requestMethod(options._requestMethod),
      _responseType(options._responseType),
      _mode(options._mode),
      _cacheType(options._cacheType),
      _credentialsType(options._credentialsType),
      _redirectType(options._redirectType),
      _referrerPolicy(options._referrerPolicy),
      _headers(options._headers)
{
    if (options._body)
    {
        _body = std::make_unique<Emval>(*options._body);
    }
    else
    {
        _body.reset();
    }
}

void FetchOptions::SetRequestMethod(const FetchRequestMethod &requestMethod)
{
    _requestMethod = requestMethod;
}

void FetchOptions::SetResponseType(const FetchResponseType &responseType)
{
    _responseType = responseType;
}

void FetchOptions::SetMode(const FetchMode &mode)
{
    _mode = mode;
}

void FetchOptions::SetCacheType(const FetchCacheType &cacheType)
{
    _cacheType = cacheType;
}

void FetchOptions::SetCredentialsType(const FetchCredentialsType &credentialsType)
{
    _credentialsType = credentialsType;
}

void FetchOptions::SetRedirectType(const FetchRedirectType &redirectType)
{
    _redirectType = redirectType;
}

void FetchOptions::SetReferrerPolicy(const FetchReferrerPolicy &referrerPolicy)
{
    _referrerPolicy = referrerPolicy;
}

void FetchOptions::SetBody(StdUniquePtr<Emval> &&body)
{
    _body = std::move(body);
}

void FetchOptions::AddHeader(const String &key, const String &value)
{
    _headers[key] = value;
}

void FetchOptions::RemoveHeader(const String &key)
{
    _headers.erase(key);
}

void FetchOptions::ClearHeaders()
{
    _headers.clear();
}

FetchRequestMethod FetchOptions::GetRequestMethod() const
{
    return _requestMethod;
}

FetchResponseType FetchOptions::GetResponseType() const
{
    return _responseType;
}

FetchMode FetchOptions::GetMode() const
{
    return _mode;
}

FetchCacheType FetchOptions::GetCacheType() const
{
    return _cacheType;
}

FetchCredentialsType FetchOptions::GetCredentialsType() const
{
    return _credentialsType;
}

FetchRedirectType FetchOptions::GetRedirectType() const
{
    return _redirectType;
}

FetchReferrerPolicy FetchOptions::GetReferrerPolicy() const
{
    return _referrerPolicy;
}

Emval FetchOptions::GetBody() const
{
    return *_body;
}

Emval FetchOptions::Build() const
{
    Emval options = Emval::object();
    if (_body)
    {
        options = Js::SetProperty(options, "body", *_body);
    }
    Js::SetProperty(options, "method", Emval(StringExtensions::ToUpper(FetchRequestMethodEnumExtensions::ToString(_requestMethod))));
    Js::SetProperty(options, "mode", Emval(StringExtensions::ToLower(StringExtensions::Replace(FetchModeEnumExtensions::ToString(_mode), "_", "-"))));
    Js::SetProperty(options, "cache", Emval(StringExtensions::ToLower(StringExtensions::Replace(FetchCacheTypeEnumExtensions::ToString(_cacheType), "_", "-"))));
    Js::SetProperty(options, "credentials", Emval(StringExtensions::ToLower(StringExtensions::Replace(FetchCredentialsTypeEnumExtensions::ToString(_credentialsType), "_", "-"))));
    Js::SetProperty(options, "redirect", Emval(StringExtensions::ToLower(StringExtensions::Replace(FetchRedirectTypeEnumExtensions::ToString(_redirectType), "_", "-"))));
    Js::SetProperty(options, "referrerPolicy", Emval(StringExtensions::ToLower(StringExtensions::Replace(FetchReferrerPolicyEnumExtensions::ToString(_referrerPolicy), "_", "-"))));
    if (_headers.size() > 0)
    {
        Emval headers = EMVAL_GLOBAL["Headers"].new_();
        for (auto value : _headers)
        {
            headers.call<void>("append", value.first, value.second);
        }
        Js::SetProperty(options, "headers", headers);
    }
    return options;
}

// ███ Fetch ███
Emval Fetch::CreateResult(const Emval &status, const Emval &data, const Emval &error)
{
    Emval result = Emval::object();
    Js::SetProperty(result, "Status", status);
    Js::SetProperty(result, "Data", data);
    Js::SetProperty(result, "Error", error);
    return result;
}

Emval Fetch::SendAsync(const String &url, const FetchOptions &options)
{
    static String statusErrorLabel = "ResponseError, Status: "s;
    EMSCRIPTEN_VOID_FUNCTOR_TYPE(2)
    promiseCallback = [url, options](Emval resolve, INSANE_MAYBE_UNUSED_ATTRIB Emval reject) -> void
    {
        EMSCRIPTEN_VOID_FUNCTOR_TYPE(1)
        errorCallback = [resolve, reject](Emval error) -> void
        {
            resolve(CreateResult(Emval(0), Emval::null(), error));
        };

        EMSCRIPTEN_VAL_FUNCTOR_TYPE(1)
        responseCallback = [options, resolve, reject](EmscriptenVal response) -> EmscriptenVal
        {
            Emval status = response["status"];
            if (!response["ok"])
            {
                resolve(CreateResult(status, Emval::null(), Js::NewError(statusErrorLabel + JsOperator::ToString(status).as<String>())));
            }
            EMSCRIPTEN_VOID_FUNCTOR_TYPE(1)
            dataCallback = [resolve, status](EmscriptenVal data) -> void
            {
                Emval result = Emval::object();
                result = Js::SetProperty(result, "Status", status);
                result = Js::SetProperty(result, "Data", data);
                result = Js::SetProperty(result, "Error", data);
                resolve(CreateResult(status, data));
            };

            return ResolveDataAsync(response, options.GetResponseType())
                .call<Emval>("then", Js::Bind(dataCallback));
        };

        EMVAL_GLOBAL["fetch"](url, options.Build())
            .call<EmscriptenVal>("then", Js::Bind(responseCallback))
            .call<EmscriptenVal>("catch", Js::Bind(errorCallback));
    };
    return EMVAL_GLOBAL["Promise"].new_(Js::Bind(promiseCallback));
}

Emval Fetch::ResolveDataAsync(const Emval &fetchResponse, const FetchResponseType &responseType)
{
    switch (responseType)
    {
        case FetchResponseType::Json:
            return fetchResponse.call<Emval>("json");
        case FetchResponseType::Text:
            return fetchResponse.call<Emval>("text");
        case FetchResponseType::ArrayBuffer:
            return fetchResponse.call<Emval>("arrayBuffer");
        case FetchResponseType::Blob:
            return fetchResponse.call<Emval>("blob");
        case FetchResponseType::FormData:
            return fetchResponse.call<Emval>("formData");
        default:
            __INSANE_THROW_EXCEPTION(NotImplementedException, "Not implemented response data type. " + FetchResponseTypeEnumExtensions::ToString(responseType), 0, nullptr, DebugType::Debug);
    }
}