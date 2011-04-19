#include "Defaults.h"

#include "TrackListControl.h"

#include "Utility.h"

using namespace std;

namespace EACRipper
{
	namespace TrackListFormatter
	{
		Formatter::Formatter(function<wstring(const wstring &)> ifn)
			: fn(ifn)
		{
		}

		Formatter::Formatter(const Formatter &fmt)
			: fn(fmt.fn)
		{
		}

		Formatter::Formatter(Formatter &&fmt)
			: fn(move(fmt.fn))
		{
		}

		wstring Formatter::run(const wstring &str)
		{
			return fn(str);
		}

		wstring Formatter::operator ()(const wstring &str)
		{
			return fn(str);
		}

		PlainFormatter::PlainFormatter()
			: Formatter(format)
		{
		}

		wstring PlainFormatter::format(const wstring &str)
		{
			return str;
		}

		TimeStringFormatter::TimeStringFormatter()
			: Formatter(format)
		{
		}

		wstring TimeStringFormatter::format(const wstring &str)
		{
			return getReadableTimeString(str);
		}
	}
}
