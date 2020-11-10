#pragma once
#ifndef INSANE_CORE_H
#define INSANE_CORE_H
#include <Insane/Insane.h>

#define USING_INSANE_CORE using namespace Insane::Core
namespace Insane::Core
{

	enum class ConsoleTextStyle
	{
		DEFAULT = 0,
		BOLD = 1,
		FAINT = 2,
		ITALIC = 3,
		UNDERLINE = 4,
		SLOW_BLINK = 5,
		RAPID_BLINK = 6,
		REVERSE = 7,
	};

	enum class ConsoleForeground
	{
		DEFAULT = 39,
		BLACK = 30,
		DARK_RED = 31,
		DARK_GREEN = 32,
		DARK_YELLOW = 33,
		DARK_BLUE = 34,
		DARK_MAGENTA = 35,
		DARK_CYAN = 36,
		GRAY = 37,
		DARK_GRAY = 90,
		RED = 91,
		GREEN = 92,
		YELLOW = 93,
		BLUE = 94,
		MAGENTA = 95,
		CYAN = 96,
		WHITE = 97
	};

	enum class ConsoleBackground
	{
		DEFAULT = 49,
		BLACK = 40,
		DARK_RED = 41,
		DARK_GREEN = 42,
		DARK_YELLOW = 43,
		DARK_BLUE = 44,
		DARK_MAGENTA = 45,
		DARK_CYAN = 46,
		GRAY = 47,
		DARK_GRAY = 100,
		RED = 101,
		GREEN = 102,
		YELLOW = 103,
		BLUE = 104,
		MAGENTA = 105,
		CYAN = 106,
		WHITE = 107
	};

	class Console
	{
	public:
		static void Clear();
		static void WriteLine(const String &s, ConsoleForeground foreground = ConsoleForeground::DEFAULT, ConsoleBackground background = ConsoleBackground::DEFAULT, std::set<ConsoleTextStyle> styles = {});
		static void Write(const String &s, ConsoleForeground foreground = ConsoleForeground::DEFAULT, ConsoleBackground background = ConsoleBackground::DEFAULT, std::set<ConsoleTextStyle> styles = {});
		static void WriteLine(const WString &s, ConsoleForeground foreground = ConsoleForeground::DEFAULT, ConsoleBackground background = ConsoleBackground::DEFAULT, std::set<ConsoleTextStyle> styles = {});
		static void Write(const WString &s, ConsoleForeground foreground = ConsoleForeground::DEFAULT, ConsoleBackground background = ConsoleBackground::DEFAULT, std::set<ConsoleTextStyle> styles = {});
		static void WriteLine();
		static void Pause();
		static int PauseAny(bool printWhenPressed = false);

	private:
		static void EnableVirtualTermimalProcessing();
		static void SetVirtualTerminalFormat(ConsoleForeground foreground, ConsoleBackground background, std::set<ConsoleTextStyle> styles);
		static void ResetTerminalFormat();
	};

	class DateTimeManager
	{
	public:
		static String CurrentISO8601DateTime(bool toUTC = true);

	private:
	};
} // namespace Insane::Core

#endif //!INSANE_CORE_H