#include <Insane/InsaneEmscripten.h>
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#include <Insane/InsanePreprocessor.h>
#include <Insane/InsaneString.h>
#include <Insane/InsaneCryptography.h>
#include <Insane/InsaneException.h>

/* Internal API */
#define INSANE_EMSCRIPTEN_KEY u8"InsaneKey"s + emscripten::val::global()[u8"navigator"][u8"userAgent"].as<String>()
#define INSANE_EMSCRIPTEN_MAIN_PROPERTY emscripten::val::global(Js::GetProperty("Insane", EMPTY_STRING, INSANE_PROPERTY_SUFFIX).c_str())
#define INSANE_PROPERTY_NAMES_KEY u8"InsanePropertyNamesKey"s //Cambiar cada cierto tiempo largo. WARNING.
#define INSANE_LOCAL_STORAGE_KEY u8"LocalStorageKey"s         //Cambiar cada cierto tiempo largo. WARNING.
#define INSANE_LOCAL_STORAGE_VOLATILE_PREFFIX u8"Volatile_"s
#define INSANE_LOCAL_STORAGE_VOLATILE_ESPECIAL_PROPERTY_SUFFIX u8"Volatile_ES_"s
#define INSANE_ESPECIAL_PROPERTY_SUFFIX u8"Property_ES_"s

namespace Insane::Emscripten::Internal
{

    bool TransformN(const int &base, int &result, std::string data, bool normal = true)
    {
        USING_EMSCRIPTEN;
        USING_INSANE_CRYPTO;
        data = HashManager::ToRawHmac(data, INSANE_EMSCRIPTEN_KEY, HashAlgorithm::SHA512);
    begin:
        if (normal)
        {
            result = (~((~base) ^ (data[0] * data[2] * data[4] * data[16] * 612)) ^ data[data.length() - 1]) ^ (base / 216);
            for (char value : data)
            {
                result -= value;
            }
            return true;
        }
        else
        {
            for (char value : data)
            {
                result += value;
            }
            result = ~((~(result ^ (base / 216))) ^ data[data.length() - 1]) ^ (data[0] * data[2] * data[4] * data[16] * 612);
            if (result == base)
            {
                return true;
            }
            else
            {
                goto begin;
            }
        }
        goto begin;
        return false;
    }

    bool AreEquals(String stra, String strb)
    {
        const int b = (Insane::Crypto::RandomManager::Generate() / 612) * 216; //base
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
void Insane::Emscripten::Operators::SetAdditionOperator()
{
    emscripten_run_script(
        u8R"(
            console.log("Add function created!!!");
            function Insane_Add(a,b)
            {
                var x;
                for(var i=1; i<2;i++)
                {
                    console.log("yes");
                }
                return a+b;
            }
        )"
    );
}

/* Promise */

emscripten::val Insane::Emscripten::Promise::Resolve(const emscripten::val &value)
{
    USING_EMSCRIPTEN;
    return val::global()[u8"Promise"].call<val>(u8"resolve", value);
}

emscripten::val Insane::Emscripten::Promise::Reject(const emscripten::val &value)
{
    USING_EMSCRIPTEN;
    return val::global()[u8"Promise"].call<val>(u8"reject", value);
}

/* Browser */
template <>
emscripten::val Insane::Emscripten::Browser::GetUserAgent()
{
    USING_EMSCRIPTEN;
    return val::global()[u8"navigator"][u8"userAgent"];
}

template <>
String Insane::Emscripten::Browser::GetUserAgent<String>()
{
    return Browser::GetUserAgent().as<String>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetTimezoneOffsetMinutes()
{
    USING_EMSCRIPTEN;
    return val(val::global()[u8"Date"].new_().call<val>(u8"getTimezoneOffset").as<int>() * -1);
}

template <>
int Insane::Emscripten::Browser::GetTimezoneOffsetMinutes()
{
    return GetTimezoneOffsetMinutes().as<int>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetTimezoneOffsetSeconds()
{
    USING_EMSCRIPTEN;
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
    USING_EMSCRIPTEN;
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
    USING_EMSCRIPTEN;
    USING_INSANE_STR;
    USING_INSANE_CRYPTO;
    String userAgent = !ua ? GetUserAgent<String>() : ua.as<String>();
    if (Strings::Contains(userAgent, u8" OPR/", false) || Strings::Contains(userAgent, u8" Opera/", false) || !!val::global()[u8"opr"] || !!val::global()[u8"opera"])
    {
        return Promise::Resolve(val(u8"Opera"));
    }

    if ((val::global()[u8"InstallTrigger"].typeOf()).as<String>() != u8"undefined" || Strings::Contains(userAgent, u8" Firefox/", false))
    {
        return Promise::Resolve(val(u8"Firefox"));
    }

    if (Strings::Contains(userAgent, u8" Edg/", false) || Strings::Contains(userAgent, u8" Edge/", false))
    {
        return Promise::Resolve(val(u8"Microsoft Edge"));
    }

    if (!!val::global()[u8"navigator"][u8"brave"])
    {
        std::function<val(val)> callback = [](val isBrave) -> val {
            return !!isBrave ? val(u8"Brave") : val(EMPTY_STRING);
        };
        return val::global()[u8"navigator"][u8"brave"].call<val>(u8"isBrave").call<val>(u8"then", Js::Bind(callback));
    }

    if (Strings::Contains(userAgent, u8" Chrome/", false) && !!val::global()[u8"chrome"])
    {
        return Promise::Resolve(val(u8"Google Chrome"));
    }

    if (Strings::Contains(userAgent, u8" Version/", false) && Strings::Contains(userAgent, u8" Safari/", false))
    {
        return Promise::Resolve(val(u8"Safari"));
    }

    return Promise::Resolve(val(EMPTY_STRING));
}

template <>
emscripten::val Insane::Emscripten::Browser::GetNameAsync(const String &ua)
{
    USING_EMSCRIPTEN;
    return GetNameAsync(ua.empty() ? GetUserAgent() : val(ua));
}

template <>
String Insane::Emscripten::Browser::GetName(const emscripten::val &ua)
{
    return GetNameAsync(ua).await().as<String>();
}

template <>
String Insane::Emscripten::Browser::GetName(const String &ua)
{
    return GetNameAsync(ua).await().as<String>();
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
//String safariIPod = "Mozilla/5.0 (iPod; CPU iPhone OS 13_1_3 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/13.0.1 Mobile/15E148 Safari/604.1";
//String safariIPad = "Mozilla/5.0 (iPad; CPU iPhone OS 13_1_3 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/13.0.1 Mobile/15E148 Safari/604.1";
//String safariIPad ="Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/13.0.1 Safari/605.1.15";
template <>
emscripten::val Insane::Emscripten::Browser::GetOS(const emscripten::val &ua)
{

    USING_EMSCRIPTEN;
    USING_INSANE_STR;
    USING_INSANE_CRYPTO;
    String userAgent = !ua ? GetUserAgent<String>() : ua.as<String>();
    if (Strings::Contains(userAgent, u8"Windows"s, false) || Strings::Contains(userAgent, u8"Win"s, false))
    {
        return val(u8"Windows"s);
    }

    if (Strings::Contains(userAgent, u8"Android"s, false))
    {
        return val(u8"Android"s);
    }

    if (Strings::Contains(userAgent, u8"Linux", false))
    {
        if (Strings::Contains(userAgent, u8"Ubuntu", false))
        {
            return val(u8"Ubuntu Linux"s);
        }
        return val(u8"Linux"s);
    }

    if (Strings::Contains(userAgent, u8"iPhone", false) ||
        Strings::Contains(userAgent, u8"iOS", false))
    {
        if (Strings::Contains(userAgent, u8"iPad", false))
        {
            return val(u8"iPadOS");
        }
        if (Strings::Contains(userAgent, u8"iPod", false))
        {
            return val(u8"iOS iPod");
        }
        return val(u8"iOS");
    }

    if (Strings::Contains(userAgent, u8"Macintosh", false) ||
        Strings::Contains(userAgent, u8"Mac", false) ||
        Strings::Contains(userAgent, u8"Mac OS X", false) ||
        Strings::Contains(userAgent, u8"Mac OS", false))
    {
        return val(u8"MacOS"s);
    }

    return val(EMPTY_STRING);
}

template <>
emscripten::val Insane::Emscripten::Browser::GetOS(const String &ua)
{
    USING_EMSCRIPTEN;
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
    USING_EMSCRIPTEN;
    return GetOS(ua.empty() ? GetUserAgent() : val(ua)).as<String>();
}

template<>
emscripten::val Insane::Emscripten::Browser::GetFingerprint()
{
    USING_EMSCRIPTEN;
    String result = Browser::GetUserAgent<String>();
    result += std::to_string(Browser::GetTimezoneOffsetMilliseconds<int>());
    return val::global()[u8"Insane"].call<val>(u8"Add", val(u8"1Hello"), val(u8"World"));
}

template<>
String Insane::Emscripten::Browser::GetFingerprint<String>()
{
    USING_EMSCRIPTEN;
}

template <>
emscripten::val Insane::Emscripten::Browser::GetWebGLRenderer()
{
    USING_EMSCRIPTEN;
    USING_INSANE_EMSCRIPTEN;
    val webGL = val::global()["document"].call<val>(u8"createElement", u8"canvas"s).call<val>(u8"getContext", u8"webgl"s);
    if (!webGL)
    {
        return val(EMPTY_STRING);
    }
    val debugInfo = webGL.call<val>(u8"getExtension", u8"WEBGL_debug_renderer_info"s);
    return !debugInfo ? val(EMPTY_STRING) : webGL.call<val>(u8"getParameter", debugInfo[u8"UNMASKED_RENDERER_WEBGL"]);
}

template <>
String Insane::Emscripten::Browser::GetWebGLRenderer<String>()
{
    USING_EMSCRIPTEN;
    return GetWebGLRenderer().as<String>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetWebGLVendor()
{
    USING_EMSCRIPTEN;
    USING_INSANE_EMSCRIPTEN;
    val webGL = val::global()["document"].call<val>(u8"createElement", u8"canvas"s).call<val>(u8"getContext", u8"webgl"s);
    if (!webGL)
    {
        return val(EMPTY_STRING);
    }
    val debugInfo = webGL.call<val>(u8"getExtension", u8"WEBGL_debug_renderer_info"s);
    return !debugInfo ? val(EMPTY_STRING) : webGL.call<val>(u8"getParameter", debugInfo[u8"UNMASKED_VENDOR_WEBGL"]);
}

template <>
String Insane::Emscripten::Browser::GetWebGLVendor<String>()
{
    USING_EMSCRIPTEN;
    return GetWebGLVendor().as<String>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetAvailableScreenHeight()
{
    USING_EMSCRIPTEN;
    return val::global()[u8"screen"][u8"availHeight"];
}

template <>
int Insane::Emscripten::Browser::GetAvailableScreenHeight()
{
    return GetAvailableScreenHeight().as<int>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetAvailableScreenWidth()
{
    USING_EMSCRIPTEN;
    return val::global()[u8"screen"][u8"availWidth"];
}

template <>
int Insane::Emscripten::Browser::GetAvailableScreenWidth()
{
    return GetAvailableScreenWidth().as<int>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetAvailableScreenOrientation()
{
    USING_EMSCRIPTEN;
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
    USING_EMSCRIPTEN;
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
    USING_EMSCRIPTEN;
    return val::global()[u8"screen"][u8"height"];
}

template <>
int Insane::Emscripten::Browser::GetScreenHeight()
{
    return GetScreenHeight().as<int>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetScreenWidth()
{
    USING_EMSCRIPTEN;
    return val::global()[u8"screen"][u8"width"];
}

template <>
int Insane::Emscripten::Browser::GetScreenWidth()
{
    return GetScreenWidth().as<int>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetScreenOrientation()
{
    USING_EMSCRIPTEN;
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
    USING_EMSCRIPTEN;
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
    USING_EMSCRIPTEN;
    return val::global()[u8"innerHeight"];
}

template <>
int Insane::Emscripten::Browser::GetViewportHeight()
{
    return GetViewportHeight().as<int>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetViewportWidth()
{
    USING_EMSCRIPTEN;
    return val::global()[u8"innerWidth"];
}

template <>
int Insane::Emscripten::Browser::GetViewportWidth()
{
    return GetViewportWidth().as<int>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetViewportOrientation()
{
    USING_EMSCRIPTEN;
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
    USING_EMSCRIPTEN;
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
    USING_EMSCRIPTEN;
    val lang = val::global()[u8"navigator"]["languages"];
    return !!lang ? lang : val::array();
}

template <>
std::vector<String> Insane::Emscripten::Browser::GetLanguages()
{
    USING_EMSCRIPTEN;
    return vecFromJSArray<String>(GetLanguages());
}

template <>
emscripten::val Insane::Emscripten::Browser::GetLanguage()
{
    USING_EMSCRIPTEN;
    return val::global()[u8"navigator"]["language"];
}

template <>
String Insane::Emscripten::Browser::GetLanguage()
{
    USING_EMSCRIPTEN;
    return GetLanguage().as<String>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetMaxTouchPoints()
{
    USING_EMSCRIPTEN;
    return val::global()[u8"navigator"][u8"maxTouchPoints"];
}

template <>
int Insane::Emscripten::Browser::GetMaxTouchPoints()
{
    USING_EMSCRIPTEN;
    return GetMaxTouchPoints().as<int>();
}

template <>
emscripten::val Insane::Emscripten::Browser::GetDeviceMemory()
{
    USING_EMSCRIPTEN;
    if (val::global()[u8"isSecureContext"].as<bool>())
    {
        val memory = val::global()[u8"navigator"][u8"deviceMemory"];
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
    USING_EMSCRIPTEN;
    val concurrency = val::global()[u8"navigator"][u8"hardwareConcurrency"];
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
    USING_EMSCRIPTEN;
    val plugins = val::global()[u8"navigator"][u8"plugins"];
    val result = val::array();
    if (!plugins)
    {
        return result;
    }
    val pluginsLength = plugins[u8"length"];
    for (int i = 0; i < plugins[u8"length"].as<int>(); i++)
    {
        val pluginValue = plugins.call<val>(u8"item", i);
        val plugin = val::object();
        plugin.set(u8"Name", !!pluginValue[u8"name"] ? pluginValue[u8"name"] : val(EMPTY_STRING));
        plugin.set(u8"Description", !!pluginValue[u8"description"] ? pluginValue[u8"description"] : val(EMPTY_STRING));
        plugin.set(u8"MimeTypes", val::array());
        val mimeTypes = val::array();
        for (int j = 0; j < pluginValue[u8"length"].as<int>(); j++)
        {
            val mimeValue = pluginValue.call<val>(u8"item", j);
            val mime = val::object();
            mime.set(u8"Description", !!mimeValue[u8"description"] ? mimeValue[u8"description"] : val(EMPTY_STRING));
            mime.set(u8"Type", !!mimeValue[u8"type"] ? mimeValue[u8"type"] : val(EMPTY_STRING));
            mime.set(u8"Suffixes", !!mimeValue[u8"suffixes"] ? mimeValue[u8"suffixes"] : val(EMPTY_STRING));
            plugin[u8"MimeTypes"].call<val>(u8"push", mime);
        }
        result.call<val>(u8"push", plugin);
    }
    return result;
}

template <>
std::vector<Insane::Emscripten::Browser::Plugin> Insane::Emscripten::Browser::GetPlugins()
{
    USING_EMSCRIPTEN;
    std::vector<Plugin> result;
    val plugins = GetPlugins();
    for (int i = 0; i < plugins[u8"length"].as<int>(); i++)
    {
        Plugin plugin;
        plugin.Name = plugins[i][u8"Name"].as<String>();
        val mimeTypes = plugins[i][u8"MimeTypes"];
        for (int j = 0; j < mimeTypes[u8"length"].as<int>(); j++)
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
    USING_EMSCRIPTEN;
    val mimeTypes = val::global()[u8"navigator"][u8"mimeTypes"];
    val result = val::global().array();
    if (!mimeTypes)
    {
        return result;
    }
    for (int i = 0; i < mimeTypes[u8"length"].as<int>(); i++)
    {
        val mimeValue = mimeTypes.call<val>(u8"item", i);
        val mime = val::object();
        mime.set(u8"Description", !!mimeValue[u8"description"] ? mimeValue[u8"description"] : val(EMPTY_STRING));
        mime.set(u8"Type", !!mimeValue[u8"type"] ? mimeValue[u8"type"] : val(EMPTY_STRING));
        mime.set(u8"Suffixes", !!mimeValue[u8"suffixes"] ? mimeValue[u8"suffixes"] : val(EMPTY_STRING));
        result.call<val>(u8"push", mime);
    }
    return result;
}

template <>
std::vector<Insane::Emscripten::Browser::MimeType> Insane::Emscripten::Browser::GetMimeTypes()
{
    USING_EMSCRIPTEN;
    std::vector<MimeType> result;
    val mimes = GetMimeTypes();
    for (int i = 0; i < mimes[u8"length"].as<int>(); i++)
    {
        MimeType mime;
        mime.Description = mimes[i][u8"Description"].as<String>();
        mime.Type = mimes[i][u8"Type"].as<String>();
        mime.Suffixes = mimes[i][u8"Suffixes"].as<String>();
        result.push_back(mime);
    }
    return result;
}

template <>
emscripten::val Insane::Emscripten::Browser::GetDoNotTrack()
{
    USING_EMSCRIPTEN;
    val dnt = val::global()[u8"navigator"][u8"doNotTrack"];
    return !!dnt ? dnt : val(u8"unspecified"s);
}

template <>
String Insane::Emscripten::Browser::GetDoNotTrack()
{
    return GetDoNotTrack().as<String>();
}

template <>
emscripten::val Insane::Emscripten::Browser::HasCookiesSupport()
{
    USING_EMSCRIPTEN;
    USING_INSANE_STR;
    bool result = true && !!val::global()[u8"navigator"][u8"cookieEnabled"];
    if (!!result)
    {
        val::global()[u8"document"].set(u8"cookie", "InsaneCookie=1"_val);
        result = Strings::Contains(val::global()[u8"document"][u8"cookie"].as<String>(), u8"InsaneCookie=");
        val::global()[u8"document"].set(u8"cookie", u8"InsaneCookie=1;expires="s + val::global()[u8"Date"].new_(1970, 1, 1, 0, 0, 0, 0).call<val>(u8"toGMTString").as<String>());
    }
    return val(result);
}

template <>
bool Insane::Emscripten::Browser::HasCookiesSupport()
{
    return HasCookiesSupport().as<bool>();
}

/* Json */
template <>
emscripten::val Insane::Emscripten::Json::Serialize(const emscripten::val &object)
{
    USING_EMSCRIPTEN;
    return val::global()["JSON"].call<val>(u8"stringify", object);
}

template <>
String Insane::Emscripten::Json::Serialize(const emscripten::val &object)
{
    return Serialize(object).as<String>();
}

/* Js */

template <>
emscripten::val Insane::Emscripten::Js::ToString(const emscripten::val &value)
{
    USING_EMSCRIPTEN;
    return val::global().call<val>(u8"String", val(value));
}

template <>
String Insane::Emscripten::Js::ToString(const emscripten::val &value)
{
    USING_EMSCRIPTEN;
    return ToString(value).as<String>();
}

void Insane::Emscripten::Js::SetVars()
{
    USING_EMSCRIPTEN;
    USING_INSANE_STR;
    if (Js::ValueIsNullOrUndefined(INSANE_EMSCRIPTEN_MAIN_PROPERTY))
    {
        val::global().set(Js::GetProperty("Insane", EMPTY_STRING, INSANE_PROPERTY_SUFFIX).c_str(), val::object());
    }
    if (Js::ValueIsNullOrUndefined(INSANE_EMSCRIPTEN_MAIN_PROPERTY[Js::GetProperty("ImportModule", EMPTY_STRING, INSANE_PROPERTY_SUFFIX)]))
    {
        emscripten_run_script((Js::GetProperty("Insane", EMPTY_STRING, INSANE_PROPERTY_SUFFIX) + "."s + Js::GetProperty("ImportModule", EMPTY_STRING, INSANE_PROPERTY_SUFFIX) + u8R"(= function(p){ return import(p); };)"s).c_str());
    }
}

String Insane::Emscripten::Js::GetProperty(const String &name, const String &key, const String &suffix)
{
    USING_INSANE_CRYPTO;
    USING_INSANE_STR;
    return (suffix.empty() ? INSANE_PROPERTY_SUFFIX : suffix + u8"_"s) + Strings::RemoveAll(HashManager::ToBase64Hmac(INSANE_PROPERTY_NAMES_KEY + name, INSANE_EMSCRIPTEN_KEY + key, HashAlgorithm::SHA512), {u8"+", u8"/", u8"="});
}

emscripten::val Insane::Emscripten::Js::Bind(const emscripten::val &fx)
{
    USING_EMSCRIPTEN;
    return fx["opcall"].call<val>("bind", fx);
}

bool Insane::Emscripten::Js::ValueIsNullOrUndefined(emscripten::val value)
{
    return value.isUndefined() || value.isNull();
}

template <>
emscripten::val Insane::Emscripten::Js::LoadScript(const emscripten::val &scriptpath, bool log)
{
    USING_EMSCRIPTEN;
    USING_INSANE_CRYPTO;
    USING_INSANE_STR;
    SetVars();
    String id = Js::GetProperty(HashManager::ToBase64Hash(scriptpath.as<String>(), HashAlgorithm::SHA512), EMPTY_STRING, INSANE_PROPERTY_SUFFIX);
    if (Js::ValueIsNullOrUndefined(INSANE_EMSCRIPTEN_MAIN_PROPERTY[Js::GetProperty("Loaded", EMPTY_STRING, INSANE_PROPERTY_SUFFIX)]))
    {
        INSANE_EMSCRIPTEN_MAIN_PROPERTY.set(Js::GetProperty("Loaded", EMPTY_STRING, INSANE_PROPERTY_SUFFIX), val::object());
    }
    if (!Js::ValueIsNullOrUndefined(INSANE_EMSCRIPTEN_MAIN_PROPERTY[Js::GetProperty("Loaded", EMPTY_STRING, INSANE_PROPERTY_SUFFIX)][id]))
    {
        std::function<void(val, val)> promiseCallback = [scriptpath, id](val resolve, val reject) {
            resolve(id);
        };
        return val::global("Promise").new_(Js::Bind(promiseCallback));
    }
    INSANE_EMSCRIPTEN_MAIN_PROPERTY[Js::GetProperty("Loaded", EMPTY_STRING, INSANE_PROPERTY_SUFFIX)].set(id, val(true));
    EMSCRIPTEN_VOID_FUNCTOR_TYPE(2)
    promiseCallback = [scriptpath, id, log](val resolve, val reject) -> void {
        val script = val::global("document").call<val>("createElement", val("script"));
        script.set("id", val(id));
        script.set("src", scriptpath);
        script.set("type", val("text/javascript"));
        EMSCRIPTEN_VOID_FUNCTOR_TYPE(1)
        onloadCallback = [scriptpath, resolve, id, log](val a) -> void {
            if (log)
            {
                Console::Log(u8"Loaded: \"%s\""s, scriptpath);
            }
            resolve(id);
        };
        script.set("onload", Js::Bind(onloadCallback));
        EMSCRIPTEN_VOID_FUNCTOR_TYPE(1)
        onerrorCallback = [scriptpath, id, reject, log](val a) -> void {
            val::global("document").call<val>(u8"getElementById", val(id)).call<void>(u8"remove");
            INSANE_EMSCRIPTEN_MAIN_PROPERTY[Js::GetProperty(u8"Loaded"s, EMPTY_STRING, INSANE_PROPERTY_SUFFIX)].delete_(id);
            if (log)
            {
                Console::Log(u8"Error Loading: \"%s\""s, scriptpath);
            }
            reject(id);
        };
        script.set("onerror", Js::Bind(onerrorCallback));
        val::global("document")["body"].call<val>("appendChild", script);
    };
    return val::global()["Promise"].new_(Js::Bind(promiseCallback));
}

template <>
emscripten::val Insane::Emscripten::Js::LoadScript(const String &scriptpath, bool log)
{
    USING_EMSCRIPTEN;
    return LoadScript(val(scriptpath), log);
}

template <>
emscripten::val Insane::Emscripten::Js::ImportModule(const emscripten::val &scriptpath)
{
    USING_EMSCRIPTEN;
    SetVars();
    return INSANE_EMSCRIPTEN_MAIN_PROPERTY.call<val>(Js::GetProperty(u8"ImportModule"s, EMPTY_STRING, INSANE_PROPERTY_SUFFIX).c_str(), scriptpath);
}

template <>
emscripten::val Insane::Emscripten::Js::ImportModule(const String &scriptpath)
{
    USING_EMSCRIPTEN;
    return ImportModule(val(scriptpath));
}

void Insane::Emscripten::Js::FreeState()
{
    USING_EMSCRIPTEN;
    USING_INSANE_STR;
    LocalStorage::RemoveValuesStartingWith(INSANE_LOCAL_STORAGE_VOLATILE_PREFFIX);
}

void Insane::Emscripten::Js::CheckSignature(const String &name, const String &sname, const String &key, const String &mae)
{
    USING_EMSCRIPTEN;
    USING_INSANE_CRYPTO;
    USING_INSANE_EXCEPTION;
    if (HashManager::ToBase64Hmac(Json::Serialize<String>(INSANE_EMSCRIPTEN_MAIN_PROPERTY[Js::GetProperty(name, key, INSANE_ESPECIAL_PROPERTY_SUFFIX)]), mae, HashAlgorithm::SHA512) != Js::ToString<String>(INSANE_EMSCRIPTEN_MAIN_PROPERTY[Js::GetProperty(sname, key, INSANE_ESPECIAL_PROPERTY_SUFFIX)]))
    {
        throw ExceptionBase("BadState CheckSignature");
        //ThrowError(u8"BadState");
    }
}

template <>
String Insane::Emscripten::LocalStorage::GetValue(const String &key, const String &password) noexcept
{
    USING_EMSCRIPTEN;
    USING_INSANE_EXCEPTION;
    USING_INSANE_CRYPTO;
    try
    {
        const val value = emscripten::val::global(u8"localStorage").call<emscripten::val>(u8"getItem", val(key));
        return value.isNull() ? EMPTY_STRING : AesManager::DecryptFromBase64(value.as<String>(), password);
    }
    catch (const Insane::Exception::ExceptionBase &ex)
    {
        Console::Log("ERROR GetValue NULL_STRING"s);
        return NULL_STRING;
    }
}

template <>
emscripten::val Insane::Emscripten::LocalStorage::GetValue(const String &key, const String &password) noexcept
{
    USING_EMSCRIPTEN;
    return val(GetValue(key, password));
}

#define lkey LocalStorage::GetValue(Js::GetProperty(u8"key"s, INSANE_EMSCRIPTEN_KEY, INSANE_LOCAL_STORAGE_VOLATILE_ESPECIAL_PROPERTY_SUFFIX), INSANE_EMSCRIPTEN_KEY)
#define lmatterEnergy LocalStorage::GetValue(Js::GetProperty(u8"MatterEnergy"s, lkey, INSANE_LOCAL_STORAGE_VOLATILE_ESPECIAL_PROPERTY_SUFFIX), INSANE_EMSCRIPTEN_KEY + Js::ToString(val::global(u8"ClientJS").new_().call<val>(u8"getFingerprint")).as<String>() + lkey)
void Insane::Emscripten::Js::CheckState()
{
    USING_EMSCRIPTEN;
    USING_INSANE_CRYPTO;
    USING_INSANE_EXCEPTION;
    val::global("console").call<void>("time", "CheckState"s);
    try
    {
        if (lkey.empty() || lmatterEnergy.empty())
        {
            Console::Log("LKEY: "s, lkey);
            Console::Log("LMATTERENERGY: "s, lmatterEnergy);
            throw ExceptionBase(u8"BadState CheckState Empty"s);
        }
        CheckSignature(u8"Browser1"s, u8"Browser1Signature"s, lkey, lmatterEnergy);
        CheckSignature(u8"Browser2"s, u8"Browser2Signature"s, lkey, lmatterEnergy);
        Console::Info(u8"Optimal state"s);
        val::global("console").call<void>("timeEnd", "CheckState"s);
    }
    catch (const ExceptionBase &e)
    {
        val::global("console").call<void>("timeEnd", "CheckState"s);
        throw ExceptionBase(u8"BadState CheckState Exception. "s + "Base: " + e.Message());
    }
}

emscripten::val Insane::Emscripten::Js::Init(const String &key)
{
    USING_EMSCRIPTEN;
    USING_INSANE_CRYPTO;
    USING_INSANE_EXCEPTION;
    //FreeState();
    SetVars();
    bool log = true;
    EMSCRIPTEN_VAL_FUNCTOR_TYPE(1)
    onFingerprintjs2LoadedCallback = [log](emscripten::val scriptId) -> emscripten::val {
        return Js::LoadScript("Libs/ClientJS/client.min.js"s, log);
    };

    EMSCRIPTEN_VAL_FUNCTOR_TYPE(1)
    onClientJSLoadedCallback = [log](emscripten::val scriptId) -> emscripten::val {
        return Js::LoadScript("Libs/Ua-parser/ua-parser.min.js"s, log);
    };

    EMSCRIPTEN_VAL_FUNCTOR_TYPE(1)
    onUaParserLoadedCallback = [log](emscripten::val scriptId) -> emscripten::val {
        return Js::ImportModule("./Libs/Bowser/bowser.js"s);
    };

    std::function<val(val)> onBowserModuleLoadedCallback = [key](emscripten::val bowserModule) -> emscripten::val {
        USING_INSANE_EMSCRIPTEN;
        val browser1 = bowserModule[u8"default"].call<val>(u8"getParser", Browser::GetUserAgent());
        val browser2 = val::global(u8"UAParser").new_(Browser::GetUserAgent());
        INSANE_EMSCRIPTEN_MAIN_PROPERTY.set(Js::GetProperty(u8"Browser1"s, key, INSANE_ESPECIAL_PROPERTY_SUFFIX), browser1);
        INSANE_EMSCRIPTEN_MAIN_PROPERTY.set(Js::GetProperty(u8"Browser2"s, key, INSANE_ESPECIAL_PROPERTY_SUFFIX), browser2);
        return val::global(u8"Fingerprint2").call<val>(u8"getPromise");
    };

    EMSCRIPTEN_VOID_FUNCTOR_TYPE(1)
    onFingerprint2LoadedCallback = [key](emscripten::val components) -> void {
        EMSCRIPTEN_VAL_FUNCTOR_TYPE(3)
        mapCallback = [](val component, val index, val array) -> val {
            return Js::ToString(component[u8"value"]);
        };
        auto matterEnergy = HashManager::ToRawHash(Js::ToString(val::global(u8"Fingerprint2").call<val>(u8"x64hash128", components.call<val>(u8"map", Js::Bind(mapCallback)).call<val>(u8"join", val(val::global(u8"ClientJS").new_().call<val>(u8"getFingerprint"))), 31)).as<String>(), HashAlgorithm::SHA512);

        Console::Log("MATTER ENERGY CALLBACK: "s, HashManager::ToBase64(matterEnergy));
        Console::Log("KEY INIT: "s, HashManager::ToBase64(key));
        INSANE_EMSCRIPTEN_MAIN_PROPERTY.set(Js::GetProperty(u8"Browser1Signature"s, key, INSANE_ESPECIAL_PROPERTY_SUFFIX), HashManager::ToBase64Hmac(Json::Serialize<String>(INSANE_EMSCRIPTEN_MAIN_PROPERTY[Js::GetProperty(u8"Browser1", key, INSANE_ESPECIAL_PROPERTY_SUFFIX)]), matterEnergy, HashAlgorithm::SHA512));
        INSANE_EMSCRIPTEN_MAIN_PROPERTY.set(Js::GetProperty(u8"Browser2Signature"s, key, INSANE_ESPECIAL_PROPERTY_SUFFIX), HashManager::ToBase64Hmac(Json::Serialize<String>(INSANE_EMSCRIPTEN_MAIN_PROPERTY[Js::GetProperty(u8"Browser2", key, INSANE_ESPECIAL_PROPERTY_SUFFIX)]), matterEnergy, HashAlgorithm::SHA512));
        LocalStorage::SetValue(Js::GetProperty(u8"MatterEnergy"s, key, INSANE_LOCAL_STORAGE_VOLATILE_ESPECIAL_PROPERTY_SUFFIX), matterEnergy, INSANE_EMSCRIPTEN_KEY + Js::ToString(val::global(u8"ClientJS").new_().call<val>(u8"getFingerprint")).as<String>() + key);
        LocalStorage::SetValue(Js::GetProperty(u8"key"s, INSANE_EMSCRIPTEN_KEY, INSANE_LOCAL_STORAGE_VOLATILE_ESPECIAL_PROPERTY_SUFFIX), key, INSANE_EMSCRIPTEN_KEY);

        Console::Info(u8"All Components Loaded"s);
        try
        {
            Js::CheckState();
        }
        catch (const ExceptionBase &e)
        {
            //RemoveAllVolatile
            Js::ThrowError(e.Message());
        }
    };

    EMSCRIPTEN_VOID_FUNCTOR_TYPE(1)
    onErrorCallback = [](emscripten::val error) {
        error.throw_();
    };

    Js::LoadScript(val(u8"Libs/Fingerprintjs2/fingerprint2.js"), log)
        .call<val>(u8"then", Js::Bind(onFingerprintjs2LoadedCallback))
        .call<val>(u8"then", Js::Bind(onClientJSLoadedCallback))
        .call<val>(u8"then", Js::Bind(onUaParserLoadedCallback))
        .call<val>(u8"then", Js::Bind(onBowserModuleLoadedCallback))
        .call<val>(u8"then", Js::Bind(onFingerprint2LoadedCallback))
        .call<val>(u8"catch", Js::Bind(onErrorCallback));
    Console::Log("Executed"s);
    return val(true);
}

void Insane::Emscripten::Js::ThrowError(const String &message)
{
    USING_EMSCRIPTEN;
    USING_INSANE_EXCEPTION;
    val::global("Error").new_(val(message)).throw_();
}

/* Console */

/* LocalStorage */

bool Insane::Emscripten::LocalStorage::SetValue(const String &key, const String &value, const String &password) noexcept
{
    USING_EMSCRIPTEN;
    USING_INSANE_EXCEPTION;
    USING_INSANE_CRYPTO;
    try
    {
        emscripten::val::global(u8"localStorage").call<void>(u8"setItem", key, AesManager::EncryptToBase64(value, password));
        return true;
    }
    catch (const ExceptionBase &e)
    {
        Console::Log("Error SetValue"s);
        return false;
    }
}

String Insane::Emscripten::LocalStorage::GetVolatileValue(const String &key, const String &password) noexcept
{
    USING_EMSCRIPTEN;
    USING_INSANE_EXCEPTION;
    USING_INSANE_CRYPTO;
    try
    {
        Js::CheckState();
    }
    catch (const ExceptionBase &e)
    {
        Js::FreeState();
        Js::ThrowError(e.Message());
    }

    String value = GetValue(INSANE_LOCAL_STORAGE_VOLATILE_PREFFIX + key, lmatterEnergy + password);
    return value;
}

bool Insane::Emscripten::LocalStorage::SetVolatileValue(const String &key, const String &value, const String &password) noexcept
{
    USING_EMSCRIPTEN;
    USING_INSANE_EXCEPTION;
    USING_INSANE_CRYPTO;
    try
    {
        Js::CheckState();
    }
    catch (const ExceptionBase &e)
    {
        Js::FreeState();
        Js::ThrowError(e.Message());
    }
    return SetValue(INSANE_LOCAL_STORAGE_VOLATILE_PREFFIX + key, value, lmatterEnergy + password);
}

template <>
void Insane::Emscripten::LocalStorage::RemoveValue(const emscripten::val &key) noexcept
{
    USING_EMSCRIPTEN;
    val::global(u8"localStorage").call<void>(u8"removeItem", key);
}

template <>
void Insane::Emscripten::LocalStorage::RemoveValue(const String &key) noexcept
{
    USING_EMSCRIPTEN;
    RemoveValue(val(key));
}

void Insane::Emscripten::LocalStorage::Clear()
{
    USING_EMSCRIPTEN;
    val::global("localStorage").call<void>(u8"clear");
}

void Insane::Emscripten::LocalStorage::RemoveValuesStartingWith(const String &preffix)
{
    USING_EMSCRIPTEN;
    USING_INSANE_STR;
    for (val value : emscripten::vecFromJSArray<val>(val::global("Object").call<val>(u8"entries", val::global("localStorage"))))
    {
        if (Strings::StartsWith(value[0].as<String>(), preffix))
        {
            LocalStorage::RemoveValue(value[0]);
        }
    }
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