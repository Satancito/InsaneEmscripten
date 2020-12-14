#pragma once
#ifndef INSANE_STRING_H
#define INSANE_STRING_H

#include <Insane/Insane.h>
#define USING_INSANE_STR using namespace Insane::Str

#define EMPTY_STRING (u8""s)
#define EMPTY_WSTRING (L""s)
#define EMPTY_STRING (u8""s)
#define NULL_STRING (u8"\0"s)
#define CR_STRING (u8"\r"s)
#define CARRIAGE_RETURN_STRING (u8"\r"s)
#define CR_WSTRING (L"\r"s)
#define LF_STRING (u8"\n"s)
#define LINE_FEED_STRING (u8"\n"s)
#define LF_WSTRING (L"\n"s)
#define CRLF_STRING (u8"\r\n"s)
#define CRLF_WSTRING (L"\r\n"s)
#define VERTICAL_TAB_STRING (u8"\v"s)
#define FORM_FEED_STRING (u8"\f"s)
#define TAB_STRING (u8"\t"s)


#define QUOTATION_MARK_STRING (u8"\"")
#define QUOTATION_MARK_WSTRING (L"\"")
#define UNDERSCORE_STRING (u8"_"s)
#define UNDERSCORE_WSTRING (L"_"s)
#define SPACE_STRING (u8" "s)
#define SPACE_WSTRING (L" "s)

#define SPACE_CHAR ' '
#define NULL_CHAR (static_cast<char>(0))
#define NULL_WCHAR (static_cast<wchar_t>(0))
#define LINE_FEED_CHAR (static_cast<char>(10))
#define LINE_FEED_WCHAR (static_cast<char>(10))

#ifdef WINDOWS_PLATFORM
#define NEW_LINE_STR u8"\r\n"
#define NEW_LINE_WSTR L"\r\n"
#elif LINUX_PLATFORM || MACOS_PLATFORM || EMSCRIPTEN_PLATFORM
#define NEW_LINE_STR u8"\n"
#define NEW_LINE_WSTR L"\n"
#endif

#define DEFAULT_LOCALE_STR u8"en_US.UTF-8"

typedef String ToFind;
typedef String ToReplace;
namespace Insane::Str {

	class Strings
	{
	public:
		Strings();
		~Strings();
		static String Empty();
		static String TrimStart(const String& data);
		static String TrimEnd(const String& data);
		static String Trim(const String& data);
		static bool IsMatch(const String &input, const String & pattern);
		static String ReplaceLastOf(const String & data, const String & toFind, const String & toReplace);
		static String ReplaceAll(const String& data, const String& toFind, const String& toReplace);
		static String ReplaceAll(const String& data, const std::initializer_list<std::pair<ToFind,ToReplace>>& pairs);
		static String RemoveAll(const String& data, const String& toRemove);
		static String RemoveAll(const String& data, const std::initializer_list <String>& toRemove);
		static String InsertRepeat(const String& data, size_t repeatEvery, const String& toRepeat, bool includeEnd = false);
		static std::vector<String> Split(const String& data, const String& toFind);
		static size_t TotalChars(const String& data);
		static String Reverse(const String& data, bool asUTF8 = true);
		static String PadRight(const String& data, const size_t& totalWidth, const char& padding = SPACE_CHAR);
		static String PadLeft(const String& data, const size_t& totalWidth, const char& padding = SPACE_CHAR);

		static WString EmptyW();
		static WString ReplaceAll(const WString& data, const WString& toFind, const WString& toReplace);
		static WString RemoveAll(const WString& data, const WString& toRemove);
		static WString InsertRepeat(const WString& data, size_t repeatEvery, const WString& toRepeat, bool includeEnd = false);
		static std::vector<WString> Split(const WString& data, const WString& toFind);
	

		static String WideStringToString(const WString& wstr);
		static WString StringToWideString(const String& str);
		static WString ToUpper(const WString& data);
		static String ToUpper(const String& data);
		static WString ToLower(const WString& data);
		static String ToLower(const String& data);

		static bool StartsWith(const String& data, const String& preffix, const bool & caseSensitive = true);
		static bool EndsWith(const String& data, const String& suffix, const bool & caseSensitive = true);
		static bool Contains(const String& data, const String& content, const bool & caseSensitive = true);

	private:
	};

#define StringsWideStringToString(data) Insane::Str::Strings::WideStringToString(data)
#define StringsStringToWideString(data) Insane::Str::Strings::StringToWideString(data)

}
#endif // !INSANE_STRING_H

