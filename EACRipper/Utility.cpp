#include "Defaults.h"

#include "Utility.h"
#include "Singleton.h"

namespace EACRipper
{
	std::wstring &trim(std::wstring &str)
	{
		static auto fn = [](wchar_t ch) { return ch != L' ' && ch != L'¡¡' && ch != L'\t' && ch != L'\n' && ch != L'\r'; };
		str.erase(str.begin(), std::find_if(str.begin(), str.end(), fn));
		str.erase(std::find_if(str.rbegin(), str.rend(), fn).base(), str.end());
		return str;
	}

	std::vector<std::wstring> split(const std::wstring &str, const std::wstring &sep)
	{
		size_t ppos, pos = static_cast<size_t>(-static_cast<ptrdiff_t>(sep.size()));
		std::vector<std::wstring> ve;

		while(true)
		{
			ppos = pos + sep.size();
			pos = str.find(sep, ppos);

			if(pos == std::wstring::npos)
			{
				ve.push_back(str.substr(ppos));
				break;
			}
			else
				ve.push_back(str.substr(ppos, pos - ppos));
		}

		return ve;
	}

	std::vector<std::wstring> splitAnyOf(const std::wstring &str, const std::wstring &sep)
	{
		size_t ppos, pos = static_cast<size_t>(-1);
		std::vector<std::wstring> ve;

		while(true)
		{
			ppos = pos;
			pos = str.find_first_of(sep, ++ ppos);

			if(pos == std::wstring::npos)
			{
				ve.push_back(str.substr(ppos));
				break;
			}
			else
				ve.push_back(str.substr(ppos, pos - ppos));
		}

		return ve;
	}

	std::wstring join(const std::vector<std::wstring> &ve, const std::wstring &sep)
	{
		if(ve.empty())
			return std::wstring();

		std::wstring str = ve.front();
		for(auto it = ++ ve.begin(); it != ve.end(); ++ it)
		{
			str += sep;
			str += *it;
		}

		return str;
	}

	int32_t getTimestamp(const std::wstring &time)
	{
		std::vector<std::wstring> times(split(time, L":"));
		if(times.size() != 3)
			return std::numeric_limits<int32_t>::max();

		return boost::lexical_cast<int32_t>(times[0]) * 60000
			+ boost::lexical_cast<int32_t>(times[1]) * 1000
			+ (boost::lexical_cast<int32_t>(times[2]) * 40 / 3);
	}

	std::wstring makeTimeString(int32_t millisec)
	{
		std::wstringstream ss;
		bool minus = millisec < 0;
		millisec = abs(millisec);
		ss << std::setfill(L'0')
			<< (minus ? L"-" : L"")
			<< std::setw(2) << (millisec / 60000)
			<< L':' << std::setw(2) << ((millisec / 1000) % 60)
			<< L':' << std::setw(2) << ((millisec % 1000) * 3 / 40);
		return ss.str();
	}

	std::wstring getTimeStringIncr(const std::wstring &time, const std::wstring &amount)
	{
		std::vector<std::wstring> times(split(time, L":")), amounts(split(amount, L":"));
		std::vector<int32_t> timesInt(3), amountsInt(3);
		if(times.size() != 3 || amounts.size() != 3)
			return std::wstring();

		std::transform(times.begin(), times.end(), timesInt.begin(), boost::lexical_cast<int32_t, std::wstring>);
		std::transform(amounts.begin(), amounts.end(), amountsInt.begin(), boost::lexical_cast<int32_t, std::wstring>);

		timesInt[2] += amountsInt[2];
		if(timesInt[2] >= 75)
		{
			timesInt[1] += timesInt[2] / 75;
			timesInt[2] %= 75;
		}

		timesInt[1] += amountsInt[1];
		if(timesInt[1] >= 60)
		{
			timesInt[0] += timesInt[1] / 60;
			timesInt[1] %= 60;
		}

		std::wstringstream ss;
		ss << std::setfill(L'0')
			<< std::setw(2) << timesInt[0]
			<< L':' << std::setw(2) << timesInt[1]
			<< L':' << std::setw(2) << timesInt[2];
		return ss.str();
	}

	std::wstring getTimeStringDiff(const std::wstring &start, const std::wstring &end)
	{
		std::vector<std::wstring> starts(split(start, L":")), ends(split(end, L":"));
		std::vector<int32_t> startsInt(3), endsInt(3);
		if(starts.size() != 3 || ends.size() != 3)
			return std::wstring();

		std::transform(starts.begin(), starts.end(), startsInt.begin(), boost::lexical_cast<int32_t, std::wstring>);
		std::transform(ends.begin(), ends.end(), endsInt.begin(), boost::lexical_cast<int32_t, std::wstring>);

		endsInt[2] -= startsInt[2];
		while(endsInt[2] < 0)
		{
			-- endsInt[1];
			endsInt[2] += 75;
		}

		endsInt[1] -= startsInt[1];
		while(endsInt[1] < 0)
		{
			-- endsInt[0];
			endsInt[1] += 60;
		}

		endsInt[0] -= startsInt[0];

		bool minus = endsInt[0] < 0;
		endsInt[0] = abs(endsInt[0]);
		std::wstringstream ss;
		ss << std::setfill(L'0')
			<< (minus ? L"-" : L"")
			<< std::setw(2) << endsInt[0]
			<< L':' << std::setw(2) << endsInt[1]
			<< L':' << std::setw(2) << endsInt[2];
		return ss.str();
	}

	std::wstring getReadableTimeString(const std::wstring &time)
	{
		int32_t timestamp = getTimestamp(time);

		bool minus = timestamp < 0;
		timestamp = abs(timestamp);

		std::wstringstream ss;
		ss << std::setfill(L'0')
			<< (minus ? L"-" : L"")
			<< std::setw(2) << (timestamp / 60000)
			<< L':' << std::setw(2) << ((timestamp / 1000) % 60)
			<< L'.' << std::setw(3) << (timestamp % 1000);
		return ss.str();
	}

	std::wstring getDirectoryPath(const std::wstring &ipath)
	{
		std::wstring path = ipath;
		size_t pos = path.rfind(L'\\');
		if(pos != std::wstring::npos)
			path.erase(++ pos);
		return path;
	}

	std::wstring getFileName(const std::wstring &ipath)
	{
		std::wstring path = ipath;
		size_t pos = path.rfind(L'\\');
		if(pos != std::wstring::npos)
			path.erase(0, ++ pos);
		return path;
	}

	std::wstring getCurrentDirectoryPath()
	{
		std::vector<wchar_t> buffer(512);
		size_t pathlen;

		pathlen = GetModuleFileNameW(nullptr, &*buffer.begin(), static_cast<DWORD>(buffer.size()));

		std::wstring path(buffer.begin(), buffer.begin() + pathlen);
		return getDirectoryPath(path);
	}

	namespace
	{
		class OSVersion : public Singleton<OSVersion>
		{
		private:
			OSVERSIONINFOW osi;

		private:
			OSVersion() { osi.dwOSVersionInfoSize = sizeof(osi); GetVersionExW(&osi); }
			~OSVersion() {}

		public:
			const OSVERSIONINFOW &getOSI() { return osi; }

			friend class Singleton<OSVersion>;
		};
	}

	bool overOSVersion(OSVERSION ver)
	{
		const OSVERSIONINFOW &osi = OSVersion::instance().getOSI();
		static const uint32_t versions[][3] = {
			{0, 0, 0},
			{VER_PLATFORM_WIN32_NT, 5, 0}, // NT2000 [1]
			{VER_PLATFORM_WIN32_NT, 5, 1}, // XP
			{VER_PLATFORM_WIN32_NT, 5, 2}, // SERVER2003
			{VER_PLATFORM_WIN32_NT, 6, 0}, // VISTA
			{VER_PLATFORM_WIN32_NT, 6, 0}, // SERVER2008
			{VER_PLATFORM_WIN32_NT, 6, 1}, // SEVEN [6]
			{VER_PLATFORM_WIN32_NT, 6, 1}, // SERVER2008R2
		};

		return osi.dwPlatformId == versions[ver][0] &&
			(osi.dwMajorVersion > versions[ver][1]
			|| (osi.dwMajorVersion == versions[ver][1] && osi.dwMinorVersion >= versions[ver][2]));
	}
}
