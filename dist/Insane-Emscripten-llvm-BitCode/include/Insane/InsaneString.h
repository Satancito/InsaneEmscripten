#pragma once
#ifndef INSANE_STRING_H
#define INSANE_STRING_H

#include <Insane/Insane.h>
#define USING_INSANE_STR using namespace Insane::Str

#define EMPTY_STR (u8"")
#define EMPTY_WSTR (L"")
#define EMPTY_STRING (u8""s)
#define NULL_STRING (u8"\0"s)

#define CR_STR (u8"\r")
#define CR_STRING (u8"\r"s)
#define CR_WSTR (L"\r")

#define LF_STR (u8"\n")
#define LF_STRING (u8"\n"s)
#define LF_WSTR (L"\n")

#define CRLF_STR (u8"\r\n")
#define CRLF_STRING (u8"\r\n"s)
#define CRLF_WSTR (L"\r\n")

#define QUOTATION_MARK_STR (u8"\"")
#define QUOTATION_MARK_WSTR (L"\"")

#define UNDERSCORE_STR (u8"_")
#define UNDERSCORE_WSTR (L"_")

#define SPACE_STR (u8" ")
#define SPACE_WSTR (L" ")

#ifdef WINDOWSLIB
#define NEW_LINE_STR u8"\r\n"
#define NEW_LINE_WSTR L"\r\n"

#elif defined LINUXLIB
#define NEW_LINE_STR u8"\n"
#define NEW_LINE_WSTR L"\n" 
#elif defined MACOSLIB
#elif defined EMSCRIPTENLIB
#define NEW_LINE_STR u8"\n"
#define NEW_LINE_WSTR L"\n"
#endif

#define DEFAULT_LOCALE_STR u8"en_US.UTF-8"

#define NULL_CHAR (static_cast<char>(0))
#define NULL_WCHAR (static_cast<wchar_t>(0))

#define LINE_FEED_CHAR (static_cast<char>(10))
#define LINE_FEED_WCHAR (static_cast<char>(10))

typedef String ToFind;
typedef String ToReplace;
namespace Insane::Str {

	class Strings
	{
	public:
		Strings();
		~Strings();
		static String Empty();
		static String ReplaceAll(const String& data, const String& toFind, const String& toReplace);
		static String ReplaceAll(const String& data, const std::initializer_list<std::pair<ToFind,ToReplace>>& pairs);
		static String RemoveAll(const String& data, const String& toRemove);
		static String RemoveAll(const String& data, const std::initializer_list <String>& toRemove);
		static String InsertRepeat(const String& data, size_t repeatEvery, const String& toRepeat, bool includeEnd = false);
		static std::vector<String> Split(const String& data, const String& toFind);
		static String ToUpper(const String& data);//TODO GESTIONAR LOCALE
		static String ToLower(const String& data);//TODO GESTIONAR LOCALE
		static size_t TotalChars(const String& data);
		static String Reverse(const String& data, bool asUTF8 = true);

		static WString EmptyW();
		static WString ReplaceAll(const WString& data, const WString& toFind, const WString& toReplace);
		static WString RemoveAll(const WString& data, const WString& toRemove);
		static WString InsertRepeat(const WString& data, size_t repeatEvery, const WString& toRepeat, bool includeEnd = false);
		static std::vector<WString> Split(const WString& data, const WString& toFind);
		static WString ToUpper(const WString& data);//TODO GESTIONAR LOCALE
		static WString ToLower(const WString& data);//TODO GESTIONAR LOCALE

		static String WideStringToString(const WString& wstr, const String& locale = DEFAULT_LOCALE_STR);
		static WString StringToWideString(const String& str, const String& locale = DEFAULT_LOCALE_STR);

		static bool StartsWith(const String& data, const String& preffix);
		static bool EndsWith(const String& data, const String& suffix);
		static bool Contains(const String& data, const String& content);

	private:

	};

#define StringsWideStringToStringF(data, locale) Insane::Str::Strings::WideStringToString(data, locale)
#define StringsStringToWideStringF(data, locale) Insane::Str::Strings::StringToWideString(data, locale)
#define StringsWideStringToString(data) Insane::Str::Strings::WideStringToString(data)
#define StringsStringToWideString(data) Insane::Str::Strings::StringToWideString(data)

}
#endif // !INSANE_STRING_H

