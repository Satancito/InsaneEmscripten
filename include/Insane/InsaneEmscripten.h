//#pragma once
#ifndef __INSANE_EMSCRIPTEN_H__
#define __INSANE_EMSCRIPTEN_H__

#include <Insane/Insane.h>
#include <Insane/InsaneString.h>
#include <Insane/InsanePreprocessor.h>
#include <emscripten/bind.h>
#include <type_traits>
#include <functional>

#define VAL_TYPE() emscripten::val

#define EMSCRIPTEN_EXPORT_FUNCTOR(arity, returnType, name, p3, p4, p5) emscripten::class_<std::function<returnType(INSANE_REPEAT_COMMA_##arity(VAL_TYPE(), 0))>>(STRINGIFY(name##arity)).constructor<>().function("opcall", &std::function<returnType(INSANE_REPEAT_COMMA_##arity(VAL_TYPE(), 0))>::operator());
#define EMSCRIPTEN_EXPORT_ALL_VOID_FUNCTORS(arity) INSANE_REPEAT_ADVANCED(EMSCRIPTEN_EXPORT_FUNCTOR, arity, VOID_TYPE(), VoidFunctor, 0, 0, 0)
#define EMSCRIPTEN_EXPORT_ALL_VAL_FUNCTORS(arity) INSANE_REPEAT_ADVANCED(EMSCRIPTEN_EXPORT_FUNCTOR, arity, VAL_TYPE(), ValFunctor, 0, 0, 0)
#define EMSCRIPTEN_EXPORT_ALL_FUNCTORS(arity) \
    EMSCRIPTEN_EXPORT_ALL_VAL_FUNCTORS(arity) \
    EMSCRIPTEN_EXPORT_ALL_VOID_FUNCTORS(arity)

#define EMSCRIPTEN_FUNCTOR_TYPE(arity, returnType) std::function<returnType(INSANE_REPEAT_COMMA_##arity(VAL_TYPE(), 0))>
#define EMSCRIPTEN_VOID_FUNCTOR_TYPE(arity) EMSCRIPTEN_FUNCTOR_TYPE(arity, VOID_TYPE())
#define EMSCRIPTEN_VAL_FUNCTOR_TYPE(arity) EMSCRIPTEN_FUNCTOR_TYPE(arity, VAL_TYPE())

#define USING_EMSCRIPTEN using namespace emscripten
#define USING_INSANE_EMSCRIPTEN using namespace Insane::Emscripten

#include <Insane/InsaneCryptography.h>
#include <Insane/InsaneException.h>

#define INSANE_PROPERTY_SUFFIX u8"Insane_"s
#define VAL_GLOBAL emscripten::val::global()
#define VAL_INSANE VAL_GLOBAL[INSANE_STRING]

static inline emscripten::val operator"" _valb(unsigned long long value)
{
    USING_EMSCRIPTEN;
    return val::global().call<val>("Boolean", value != 0);
}

static inline emscripten::val operator"" _val(unsigned long long value)
{
    USING_EMSCRIPTEN;
    return val::global().call<val>("parseInt", std::to_string(value));
}

static inline emscripten::val operator"" _val(long double value)
{
    USING_EMSCRIPTEN;
    return val::global().call<val>("parseFloat", std::to_string(value));
}

static inline emscripten::val operator"" _val(const char *value, size_t size)
{
    USING_EMSCRIPTEN;
    return val(String(value, size));
}

static inline emscripten::val operator"" _val(char value)
{
    USING_EMSCRIPTEN;
    return val(value);
}

typedef emscripten::val EmscriptenVal;

namespace Insane::Emscripten
{

    class Operator final
    {
    private:
        enum class OperatorArityType
        {
            Unary,
            Binary
        };

        enum class OperatorType
        {
            Addition,
            Subtraction,
            Multiplication,
            Division,
            Import,
            TypeOf,
            IsNull,
            IsUndefined,
            IsNullOrUndefined
        };

        static EmscriptenVal CallOperator(const EmscriptenVal &a, const EmscriptenVal &b, const OperatorType &operatorType, const OperatorArityType &operatorArityType);

    public:
        [[nodiscard]] static EmscriptenVal Add(const EmscriptenVal &a, const EmscriptenVal &b);
        [[nodiscard]] static EmscriptenVal Subtract(const EmscriptenVal &a, const EmscriptenVal &b);
        [[nodiscard]] static EmscriptenVal Multiply(const EmscriptenVal &a, const EmscriptenVal &b);
        [[nodiscard]] static EmscriptenVal Divide(const EmscriptenVal &a, const EmscriptenVal &b);
        [[nodiscard]] static EmscriptenVal ImportAsync(const EmscriptenVal &a);
        [[nodiscard]] static EmscriptenVal TypeOf(const EmscriptenVal &a);
        [[nodiscard]] static bool IsNull(const EmscriptenVal &a);
        [[nodiscard]] static bool IsUndefined(const EmscriptenVal &a);
        [[nodiscard]] static bool IsNullOrUndefined(const EmscriptenVal &a);
    };

    class Converter final
    {
    private:
    public:
        template <typename ReturnType = emscripten::val,
                  typename ParamType = emscripten::val,
                  typename = typename std::void_t<std::enable_if_t<std::is_same_v<ParamType, std::string> ||
                                                                   std::is_same_v<ParamType, emscripten::val> ||
                                                                   std::is_floating_point_v<ParamType> ||
                                                                   std::is_integral_v<ParamType>>>,
                  typename = typename std::enable_if_t<std::is_same_v<ReturnType, String> ||
                                                       std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType ToString(const ParamType &value);
    };

    class Promise final
    {
    private:
    public:
        [[nodiscard]] static EmscriptenVal New(const EMSCRIPTEN_VOID_FUNCTOR_TYPE(2) & promiseCallback);

        template <typename ParamType = EmscriptenVal,
                  typename = typename std::void_t<std::enable_if_t<std::is_same_v<ParamType, String> ||
                                                                   std::is_floating_point_v<ParamType> ||
                                                                   std::is_integral_v<ParamType> ||
                                                                   std::is_same_v<ParamType, EmscriptenVal>>>>
        [[nodiscard]] static inline EmscriptenVal Resolve(const ParamType &value)
        {
            USING_EMSCRIPTEN;
            return val::global()[u8"Promise"].call<val>(u8"resolve", EmscriptenVal(value));
        }

        template <typename ParamType = EmscriptenVal,
                  typename = typename std::void_t<std::enable_if_t<std::is_same_v<ParamType, String> ||
                                                                   std::is_floating_point_v<ParamType> ||
                                                                   std::is_integral_v<ParamType> ||
                                                                   std::is_same_v<ParamType, EmscriptenVal>>>>
        [[nodiscard]] static EmscriptenVal Reject(const ParamType &value)
        {
            USING_EMSCRIPTEN;
            return val::global()[u8"Promise"].call<val>(u8"reject", EmscriptenVal(value));
        }
    };

    class Console final
    {
    private:
        enum class ConsoleMessageType
        {
            LOG,
            INFO,
            WARN,
            ERROR
        };
        template <typename... ParamType,
                  typename = typename std::void_t<std::enable_if_t<std::is_same_v<ParamType, std::string> ||
                                                                   std::is_same_v<ParamType, emscripten::val> ||
                                                                   std::is_floating_point_v<ParamType> ||
                                                                   std::is_integral_v<ParamType>>...>>
        static inline void Print(ConsoleMessageType type, const ParamType &... args)
        {
            USING_EMSCRIPTEN;
            String method = EMPTY_STRING;
            switch (type)
            {
            case ConsoleMessageType::INFO:
                method = u8"info"s;
                break;
            case ConsoleMessageType::WARN:
                method = u8"warn"s;
                break;
            case ConsoleMessageType::ERROR:
                method = u8"error"s;
                break;
            default:
                method = u8"log";
                break;
            }
            val::global("console").call<void>(method.c_str(), args...);
        };

    public:
        Console() = default;
        ~Console() = default;
        template <typename... ParamType,
                  typename = typename std::void_t<std::enable_if_t<std::is_same_v<ParamType, std::string> ||
                                                                   std::is_same_v<ParamType, emscripten::val> ||
                                                                   std::is_floating_point_v<ParamType> ||
                                                                   std::is_integral_v<ParamType>>...>>
        static inline void Log(const ParamType &... args)
        {
            USING_EMSCRIPTEN;
            Print(ConsoleMessageType::LOG, args...);
        };

        template <typename... ParamType,
                  typename = typename std::void_t<std::enable_if_t<std::is_same_v<ParamType, std::string> ||
                                                                   std::is_same_v<ParamType, emscripten::val> ||
                                                                   std::is_floating_point_v<ParamType> ||
                                                                   std::is_integral_v<ParamType>>...>>
        static inline void Info(const ParamType &... args)
        {
            USING_EMSCRIPTEN;
            Print(ConsoleMessageType::INFO, args...);
        };

        template <typename... ParamType,
                  typename = typename std::void_t<std::enable_if_t<std::is_same_v<ParamType, std::string> ||
                                                                   std::is_same_v<ParamType, emscripten::val> ||
                                                                   std::is_floating_point_v<ParamType> ||
                                                                   std::is_integral_v<ParamType>>...>>
        static inline void Warn(const ParamType &... args)
        {
            USING_EMSCRIPTEN;
            Print(ConsoleMessageType::WARN, args...);
        };

        template <typename... ParamType,
                  typename = typename std::void_t<std::enable_if_t<std::is_same_v<ParamType, std::string> ||
                                                                   std::is_same_v<ParamType, emscripten::val> ||
                                                                   std::is_floating_point_v<ParamType> ||
                                                                   std::is_integral_v<ParamType>>...>>
        static inline void Error(const ParamType &... args)
        {
            USING_EMSCRIPTEN;
            Print(ConsoleMessageType::ERROR, args...);
        };
    };

    template <typename T>
    struct DefaultValue;

    template <>
    struct DefaultValue<String>
    {
        static String value()
        {
            return EMPTY_STRING;
        }
    };

    template <>
    struct DefaultValue<emscripten::val>
    {
        static emscripten::val value()
        {
            return emscripten::val::null();
        }
    };

    enum class OrientationType
    {
        LANDSCAPE = 0,
        PORTRAIT = 1,
        SQUARE = 2
    };

    class Browser final
    {
    private:
    public:
        template <typename ReturnType = emscripten::val,
                  typename = typename std::enable_if_t<std::is_same_v<ReturnType, String> || std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetUserAgent();

        template <typename ReturnType = emscripten::val,
                  typename = typename std::enable_if_t<std::is_same_v<ReturnType, int> || std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetTimezoneOffsetMinutes();

        template <typename ReturnType = emscripten::val,
                  typename = typename std::enable_if_t<std::is_same_v<ReturnType, int> || std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetTimezoneOffsetSeconds();

        template <typename ReturnType = emscripten::val,
                  typename = typename std::enable_if_t<std::is_same_v<ReturnType, int> || std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetTimezoneOffsetMilliseconds();

        template <typename ParamType = emscripten::val,
                  typename = typename std::enable_if_t<std::is_same_v<ParamType, String> || std::is_same_v<ParamType, emscripten::val>>>
        [[nodiscard]] static emscripten::val GetNameAsync(const ParamType &userAgent = DefaultValue<ParamType>::value());

        template <typename ReturnType = emscripten::val,
                  typename ParamType = emscripten::val,
                  typename = typename std::enable_if_t<std::is_same_v<ReturnType, String> || std::is_same_v<ReturnType, emscripten::val>>,
                  typename = typename std::enable_if_t<std::is_same_v<ParamType, String> || std::is_same_v<ParamType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetOS(const ParamType &userAgent = DefaultValue<ParamType>::value());

        template <typename ReturnType = emscripten::val,
                  typename = typename std::enable_if_t<std::is_same_v<ReturnType, String> || std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetWebGLVendor();

        template <typename ReturnType = emscripten::val, typename = typename std::enable_if_t<std::is_same_v<ReturnType, String> || std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetWebGLRenderer();

        template <typename ReturnType = emscripten::val,
                  typename = typename std::enable_if_t<std::is_same_v<ReturnType, int> || std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetScreenHeight();

        template <typename ReturnType = emscripten::val,
                  typename = typename std::enable_if_t<std::is_same_v<ReturnType, int> || std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetScreenWidth();

        template <typename ReturnType = emscripten::val,
                  typename = typename std::enable_if_t<std::is_same_v<ReturnType, int> || std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetScreenSize();

        template <typename ReturnType = emscripten::val,
                  typename = typename std::enable_if_t<std::is_same_v<ReturnType, OrientationType> || std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetScreenOrientation();

        template <typename ReturnType = emscripten::val,
                  typename = typename std::enable_if_t<std::is_same_v<ReturnType, int> || std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetAvailableScreenHeight();

        template <typename ReturnType = emscripten::val,
                  typename = typename std::enable_if_t<std::is_same_v<ReturnType, int> || std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetAvailableScreenWidth();

        template <typename ReturnType = emscripten::val,
                  typename = typename std::enable_if_t<std::is_same_v<ReturnType, int> || std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetAvailableScreenSize();

        template <typename ReturnType = emscripten::val,
                  typename = typename std::enable_if_t<std::is_same_v<ReturnType, OrientationType> || std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetAvailableScreenOrientation();

        template <typename ReturnType = emscripten::val,
                  typename = typename std::enable_if_t<std::is_same_v<ReturnType, int> || std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetViewportWidth();

        template <typename ReturnType = emscripten::val,
                  typename = typename std::enable_if_t<std::is_same_v<ReturnType, int> || std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetViewportHeight();

        template <typename ReturnType = emscripten::val,
                  typename = typename std::enable_if_t<std::is_same_v<ReturnType, int> || std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetViewportSize();

        template <typename ReturnType = emscripten::val,
                  typename = typename std::enable_if_t<std::is_same_v<ReturnType, OrientationType> || std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetViewportOrientation();

        template <typename ReturnType = emscripten::val, typename = typename std::enable_if_t<std::is_same_v<ReturnType, String> ||
                                                                                              std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetLanguage();

        template <typename ReturnType = emscripten::val,
                  typename = typename std::enable_if_t<std::is_same_v<ReturnType, std::vector<String>> ||
                                                       std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetLanguages();

        template <typename ReturnType = emscripten::val, typename = typename std::enable_if_t<std::is_same_v<ReturnType, int> ||
                                                                                              std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetMaxTouchPoints();

        template <typename ReturnType = emscripten::val, typename = typename std::enable_if_t<std::is_same_v<ReturnType, float> ||
                                                                                              std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetDeviceMemory();

        template <typename ReturnType = emscripten::val, typename = typename std::enable_if_t<std::is_same_v<ReturnType, int> ||
                                                                                              std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetHardwareConcurrency();

        struct MimeType
        {
            String Type;
            String Description;
            String Suffixes;
        };

        struct Plugin
        {
            String Name;
            std::vector<MimeType> MimeTypes;
        };

        template <typename ReturnType = emscripten::val, typename = typename std::enable_if_t<std::is_same_v<ReturnType, std::vector<Plugin>> ||
                                                                                              std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetPlugins();

        template <typename ReturnType = emscripten::val, typename = typename std::enable_if_t<std::is_same_v<ReturnType, std::vector<MimeType>> ||
                                                                                              std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetMimeTypes();

        template <typename ReturnType = emscripten::val, typename = typename std::enable_if_t<std::is_same_v<ReturnType, String> ||
                                                                                              std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetDoNotTrack();

        template <typename ReturnType = emscripten::val, typename = typename std::enable_if_t<std::is_same_v<ReturnType, bool> ||
                                                                                              std::is_same_v<ReturnType, emscripten::val>>>
        [[nodiscard]] static ReturnType HasCookiesSupport();

        template <typename ParamType = emscripten::val,
                  typename = typename std::enable_if_t<std::is_same_v<ParamType, String> || std::is_same_v<ParamType, emscripten::val>>>
        [[nodiscard]] static EmscriptenVal GetFingerprintAsync(const ParamType &key);

        Browser() = delete;
        ~Browser() = delete;
    };

    class Json final
    {
    private:
    public:
        template <typename ReturnType = emscripten::val, typename = typename std::enable_if_t<std::is_same_v<ReturnType, String> ||
                                                                                              std::is_same_v<ReturnType, emscripten::val>>>
        static ReturnType Serialize(const emscripten::val &object);
    };

    class Js final
    {
    private:
        static emscripten::val Bind(const emscripten::val &fx);
        //static void SetVars();
        //static void CheckSignature(const String &name, const String &sname, const String &key, const String &mae);

    public:
        //static void FreeState();
        //static void CheckState() noexcept(false);
        static void SetPropertyNull(EmscriptenVal object, const String &property, const bool &replaceIfExists = true);
        static void SetPropertyObject(EmscriptenVal object, const String &property, const bool &replaceIfExists = true);
        static void SetPropertyArray(EmscriptenVal object, const String &property, const bool &replaceIfExists = true);
        static void SetProperty(EmscriptenVal object, const String &property, const EmscriptenVal &value, const bool &replaceIfExists = true);
        static String GetProperty(const String &name, const String &key = EMPTY_STRING, const String &suffix = INSANE_PROPERTY_SUFFIX);
        template <typename ReturnType,
                  typename... ParamType,
                  typename = typename std::void_t<std::enable_if_t<std::is_same_v<ReturnType, void> ||
                                                                   std::is_same_v<ReturnType, emscripten::val>>>,
                  typename = typename std::void_t<std::enable_if_t<std::is_same_v<ParamType, emscripten::val>>...>>
        static inline emscripten::val Bind(const std::function<ReturnType(ParamType...)> &fx)
        {
            USING_EMSCRIPTEN;
            return Bind(val(fx));
        }

        template <typename ParamType = EmscriptenVal,
                  typename = typename std::void_t<std::enable_if_t<std::is_same_v<ParamType, EmscriptenVal> ||
                                                                   std::is_same_v<ParamType, String>>>>
        static emscripten::val LoadScriptAsync(const ParamType &scriptpath);

        static void ThrowError(const String &message = "An error has ocurred.");
        // static emscripten::val Init(const String &key);
    };

    class LocalStorage final
    {
    private:
    public:
        template <typename ReturnType = EmscriptenVal,
                  typename ParamsType = EmscriptenVal,
                  typename = typename std::enable_if_t<std::is_same_v<ReturnType, String> ||
                                                       std::is_same_v<ReturnType, emscripten::val>>,
                  typename = typename std::enable_if_t<std::is_same_v<ParamsType, String>  ||
                                                       std::is_same_v<ParamsType, emscripten::val>>>
        [[nodiscard]] static ReturnType GetValue(const ParamsType &key, const ParamsType &password = DefaultValue<ParamsType>::value()) noexcept;

        template <typename ParamsType = EmscriptenVal,
                  typename = typename std::enable_if_t<std::is_same_v<ParamsType, String>  ||
                                                       std::is_same_v<ParamsType, emscripten::val>>>
        static void SetValue(const ParamsType &key, const ParamsType &value, const ParamsType &password = DefaultValue<ParamsType>::value()) noexcept(false);

        template <typename ParamType,
                  typename = typename std::void_t<std::enable_if_t<std::is_same_v<ParamType, std::string> ||
                                                                   std::is_same_v<ParamType, emscripten::val>>>>
        static void RemoveValue(const ParamType &key) noexcept;

        static void Clear();

        template <typename ParamType,
                  typename = typename std::void_t<std::enable_if_t<std::is_same_v<ParamType, std::string> ||
                                                                   std::is_same_v<ParamType, emscripten::val>>>>
        static void RemoveValuesStartingWith(const ParamType &preffix);
    };

} // namespace Insane::Emscripten

#endif // __INSANE_EMSCRIPTEN_H__

//  template <typename Type>
//         static inline constexpr std::string_view
//         type_name()
//         {
//             std::string_view name, prefix, suffix;
// #ifdef __clang__
//             name = __PRETTY_FUNCTION__;
//             prefix = "std::string_view type_name() [T = ";
//             suffix = "]";
// #elif defined(__GNUC__)
//             name = __PRETTY_FUNCTION__;
//             prefix = "constexpr std::string_view type_name() [with T = ";
//             suffix = "; std::string_view = std::basic_string_view<char>]";
// #elif defined(_MSC_VER)
//             name = __FUNCSIG__;
//             prefix = "class std::basic_string_view<char,struct std::char_traits<char> > __cdecl type_name<";
//             suffix = ">(void)";
// #endif
//             //name.remove_prefix(prefix.size()-2);
//             //name.remove_suffix(suffix.size());
//             return name;
//         }

//         template <typename T>
//         static inline void Print(std::ostream &out, const T &head)
//         {
//             out << head << std::endl;
//         }

//         template <typename T, typename... Args>
//         static inline void Print(std::ostream &out, const T &head, const Args &... args)
//         {
//             out << head << "," << std::endl;
//             Print(out, args...);
//         }