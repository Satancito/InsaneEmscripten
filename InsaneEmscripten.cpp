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

// template<>
// String Insane::Emscripten::Browser::GetName()
// {
//     USING_EMSCRIPTEN;
//     USING_INSANE_STR;
//     USING_INSANE_CRYPTO;
    


// }

// template<>
// String Insane::Emscripten::Browser::GetName()
// {
//     return GetName().as<String>();
// }

template<>
emscripten::val Insane::Emscripten::Browser::GetName(const emscripten::val& ua)
{
    USING_EMSCRIPTEN;
    USING_INSANE_STR;
    USING_INSANE_CRYPTO;
    String userAgent =  !ua ? GetUserAgent<String>(): ua.as<String>();
    if(Strings::Contains(userAgent, u8" OPR/") || Strings::Contains(userAgent, u8" Opera/") || !!val::global()[u8"opr"] || !!val::global()[u8"opera"])
    {
        return val(u8"Opera");
    }
    //typeof InstallTrigger !== 'undefined';
    if((val::global()[u8"InstallTrigger"].typeOf()).as<String>() != u8"undefined" || Strings::Contains(userAgent, u8" Firefox/"))
    {
        return val(u8"Firefox");
    }

    if(Strings::Contains(userAgent, u8" Edg/") || Strings::Contains(userAgent, u8" Edge/"))
    {
        return val(u8"Microsoft Edge");
    }

    if(!!val::global()[u8"navigator"][u8"brave"] && !!(val::global()[u8"navigator"][u8"brave"].call<val>(u8"isBrave").await()))
    {
        return val(u8"Brave"s);
    }
    
    return val(EMPTY_STRING);
}

template<>
String Insane::Emscripten::Browser::GetName(const emscripten::val& ua)
{
    return GetName(ua).as<String>();
}

template<>
emscripten::val Insane::Emscripten::Browser::GetName(const String& ua)
{
    USING_EMSCRIPTEN;
    return GetName(ua.empty() ? GetUserAgent() : val(ua));
}

template<>
String Insane::Emscripten::Browser::GetName(const String& ua)
{
    return GetName(ua).as<String>();
}

// template<>
// String Insane::Emscripten::Browser::GetFingerprint<String>()
// {
//     USING_EMSCRIPTEN;
// }

// template<>
// emscripten::val Insane::Emscripten::Browser::GetFingerprint()
// {
//     USING_EMSCRIPTEN;
// }


template<>
emscripten::val Insane::Emscripten::Browser::GetWebGLRenderer()
{
    USING_EMSCRIPTEN;
    USING_INSANE_EMSCRIPTEN;
    val webGL = val::global()["document"].call<val>(u8"createElement", u8"canvas"s).call<val>(u8"getContext", u8"webgl"s);
    if(!webGL)
    {    
        return val(EMPTY_STRING);
    }
    val debugInfo = webGL.call<val>(u8"getExtension", u8"WEBGL_debug_renderer_info"s);
    return !debugInfo ? val(EMPTY_STRING) : webGL.call<val>(u8"getParameter", debugInfo[u8"UNMASKED_RENDERER_WEBGL"]);
}

template<>
String Insane::Emscripten::Browser::GetWebGLRenderer<String>()
{
    USING_EMSCRIPTEN;
    return GetWebGLRenderer().as<String>();
}

template<>
emscripten::val Insane::Emscripten::Browser::GetWebGLVendor()
{
    USING_EMSCRIPTEN;
    USING_INSANE_EMSCRIPTEN;
    val webGL = val::global()["document"].call<val>(u8"createElement", u8"canvas"s).call<val>(u8"getContext", u8"webgl"s);
    if(!webGL)
    {    
        return val(EMPTY_STRING);
    }
    val debugInfo = webGL.call<val>(u8"getExtension", u8"WEBGL_debug_renderer_info"s);
    return !debugInfo ? val(EMPTY_STRING) : webGL.call<val>(u8"getParameter", debugInfo[u8"UNMASKED_VENDOR_WEBGL"]);
}

template<>
String Insane::Emscripten::Browser::GetWebGLVendor<String>()
{
    USING_EMSCRIPTEN;
    return GetWebGLVendor().as<String>();
}

template<>
emscripten::val Insane::Emscripten::Browser::GetLanguages(){
    USING_EMSCRIPTEN;
    val lang = val::global()[u8"navigator"]["languages"];
    return !!lang? lang: val::array();
}

template<>
std::vector<String> Insane::Emscripten::Browser::GetLanguages(){
    USING_EMSCRIPTEN;
    return vecFromJSArray<String>(GetLanguages());
}

/* Json */
template <>
emscripten::val Insane::Emscripten::Json::ToString(const emscripten::val &object)
{
    USING_EMSCRIPTEN;
    return val::global()["JSON"].call<val>(u8"stringify", object);
}

template <>
String Insane::Emscripten::Json::ToString(const emscripten::val &object)
{
    return ToString(object).as<String>();
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
        EMSCRIPTEN_VOID_FUNCTOR_TYPE(1) onerrorCallback = [scriptpath, id, reject, log](val a) -> void {
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
    if (HashManager::ToBase64Hmac(Json::ToString<String>(INSANE_EMSCRIPTEN_MAIN_PROPERTY[Js::GetProperty(name, key, INSANE_ESPECIAL_PROPERTY_SUFFIX)]), mae, HashAlgorithm::SHA512) != Js::ToString<String>(INSANE_EMSCRIPTEN_MAIN_PROPERTY[Js::GetProperty(sname, key, INSANE_ESPECIAL_PROPERTY_SUFFIX)]))
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
        return Js::LoadScript("Libs/ClientJS/client.min.js"_val, log);
    };

    EMSCRIPTEN_VAL_FUNCTOR_TYPE(1)
    onClientJSLoadedCallback = [log](emscripten::val scriptId) -> emscripten::val {
        return Js::LoadScript("Libs/Ua-parser/ua-parser.min.js"_val, log);
    };

    EMSCRIPTEN_VAL_FUNCTOR_TYPE(1)
    onUaParserLoadedCallback = [log](emscripten::val scriptId) -> emscripten::val {
        return Js::ImportModule("./Libs/Bowser/bowser.js"_val);
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
        INSANE_EMSCRIPTEN_MAIN_PROPERTY.set(Js::GetProperty(u8"Browser1Signature"s, key, INSANE_ESPECIAL_PROPERTY_SUFFIX), HashManager::ToBase64Hmac(Json::ToString<String>(INSANE_EMSCRIPTEN_MAIN_PROPERTY[Js::GetProperty(u8"Browser1", key, INSANE_ESPECIAL_PROPERTY_SUFFIX)]), matterEnergy, HashAlgorithm::SHA512));
        INSANE_EMSCRIPTEN_MAIN_PROPERTY.set(Js::GetProperty(u8"Browser2Signature"s, key, INSANE_ESPECIAL_PROPERTY_SUFFIX), HashManager::ToBase64Hmac(Json::ToString<String>(INSANE_EMSCRIPTEN_MAIN_PROPERTY[Js::GetProperty(u8"Browser2", key, INSANE_ESPECIAL_PROPERTY_SUFFIX)]), matterEnergy, HashAlgorithm::SHA512));
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

void Insane::Emscripten::LocalStorage::RemoveValuesStartingWith(const String& preffix)
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