#include "Defaults.h"

#include "Utility.h"
#include "Singleton.h"

using namespace std;
using namespace boost;

namespace EACRipper
{
	wstring &trim(wstring &str)
	{
		static auto fn = [](wchar_t ch) { return ch != L' ' && ch != L'¡¡' && ch != L'\t' && ch != L'\n' && ch != L'\r'; };
		str.erase(str.begin(), find_if(str.begin(), str.end(), fn));
		str.erase(find_if(str.rbegin(), str.rend(), fn).base(), str.end());
		return str;
	}

	vector<wstring> split(const wstring &str, const wstring &sep)
	{
		size_t ppos, pos = static_cast<size_t>(-static_cast<ptrdiff_t>(sep.size()));
		vector<wstring> ve;

		while(true)
		{
			ppos = pos + sep.size();
			pos = str.find(sep, ppos);

			if(pos == wstring::npos)
			{
				ve.push_back(str.substr(ppos));
				break;
			}
			else
				ve.push_back(str.substr(ppos, pos - ppos));
		}

		return ve;
	}

	vector<wstring> splitAnyOf(const wstring &str, const wstring &sep)
	{
		size_t ppos, pos = static_cast<size_t>(-1);
		vector<wstring> ve;

		while(true)
		{
			ppos = pos;
			pos = str.find_first_of(sep, ++ ppos);

			if(pos == wstring::npos)
			{
				ve.push_back(str.substr(ppos));
				break;
			}
			else
				ve.push_back(str.substr(ppos, pos - ppos));
		}

		return move(ve);
	}

	wstring join(const vector<wstring> &ve, const wstring &sep)
	{
		if(ve.empty())
			return wstring();

		wstring str = ve.front();
		for(auto it = ++ ve.begin(); it != ve.end(); ++ it)
		{
			str += sep;
			str += *it;
		}

		return move(str);
	}

	int32_t getTimestamp(const wstring &time)
	{
		vector<wstring> times(split(time, L":"));
		if(times.size() != 3)
			return numeric_limits<int32_t>::max();

		return lexical_cast<int32_t>(times[0]) * 60000
			+ lexical_cast<int32_t>(times[1]) * 1000
			+ (lexical_cast<int32_t>(times[2]) * 40 / 3);
	}

	wstring makeTimeString(int32_t millisec)
	{
		wstringstream ss;
		bool minus = millisec < 0;
		millisec = abs(millisec);
		ss << setfill(L'0')
			<< (minus ? L"-" : L"")
			<< setw(2) << (millisec / 60000)
			<< L':' << setw(2) << ((millisec / 1000) % 60)
			<< L':' << setw(2) << ((millisec % 1000) * 3 / 40);
		return ss.str();
	}

	wstring getTimeStringIncr(const wstring &time, const wstring &amount)
	{
		vector<wstring> times(split(time, L":")), amounts(split(amount, L":"));
		vector<int32_t> timesInt(3), amountsInt(3);
		if(times.size() != 3 || amounts.size() != 3)
			return wstring();

		transform(times.begin(), times.end(), timesInt.begin(), lexical_cast<int32_t, wstring>);
		transform(amounts.begin(), amounts.end(), amountsInt.begin(), lexical_cast<int32_t, wstring>);

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

		wstringstream ss;
		ss << setfill(L'0')
			<< setw(2) << timesInt[0]
			<< L':' << setw(2) << timesInt[1]
			<< L':' << setw(2) << timesInt[2];
		return ss.str();
	}

	wstring getTimeStringDiff(const wstring &start, const wstring &end)
	{
		vector<wstring> starts(split(start, L":")), ends(split(end, L":"));
		vector<int32_t> startsInt(3), endsInt(3);
		if(starts.size() != 3 || ends.size() != 3)
			return wstring();

		transform(starts.begin(), starts.end(), startsInt.begin(), lexical_cast<int32_t, wstring>);
		transform(ends.begin(), ends.end(), endsInt.begin(), lexical_cast<int32_t, wstring>);

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
		wstringstream ss;
		ss << setfill(L'0')
			<< (minus ? L"-" : L"")
			<< setw(2) << endsInt[0]
			<< L':' << setw(2) << endsInt[1]
			<< L':' << setw(2) << endsInt[2];
		return ss.str();
	}

	wstring getReadableTimeString(const wstring &time)
	{
		int32_t timestamp = getTimestamp(time);

		bool minus = timestamp < 0;
		timestamp = abs(timestamp);

		wstringstream ss;
		ss << setfill(L'0')
			<< (minus ? L"-" : L"")
			<< setw(2) << (timestamp / 60000)
			<< L':' << setw(2) << ((timestamp / 1000) % 60)
			<< L'.' << setw(3) << (timestamp % 1000);
		return ss.str();
	}

	wstring getDirectoryPath(const wstring &ipath)
	{
		wstring path = ipath;
		size_t pos = path.rfind(L'\\');
		if(pos != wstring::npos)
			path.erase(++ pos);
		return path;
	}

	wstring getFileName(const wstring &ipath)
	{
		wstring path = ipath;
		size_t pos = path.rfind(L'\\');
		if(pos != wstring::npos)
			path.erase(0, ++ pos);
		return path;
	}

	wstring getCurrentDirectoryPath()
	{
		vector<wchar_t> buffer(512);
		size_t pathlen;

		pathlen = GetModuleFileNameW(nullptr, &*buffer.begin(), static_cast<DWORD>(buffer.size()));

		wstring path(buffer.begin(), buffer.begin() + pathlen);
		return getDirectoryPath(path);
	}

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
