#pragma once
#include <Insane/Insane.h>
#include <Insane/InsaneString.h>

#define USING_INSANE_CORE using namespace Insane::Core
namespace Insane::Core
{
	class DateTimeManager
	{
	public:
		static String CurrentISO8601DateTime(bool toUTC = true);//https://es.wikipedia.org/wiki/ISO_8601
	private:
	};
}