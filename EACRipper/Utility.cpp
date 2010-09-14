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
			+ lexical_cast<int32_t>(times[2]) * 10;
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
			<< L':' << setw(2) << ((millisec / 10) % 100);
		return ss.str();
	}

	wstring getTimeStringDiff(const wstring &start, const wstring &end)
	{
		int32_t startTS = getTimestamp(start);
		int32_t endTS = getTimestamp(end);
		if(startTS == numeric_limits<int32_t>::max() || endTS == numeric_limits<int32_t>::max())
			return wstring();

		return makeTimeString(endTS - startTS);
	}

	wstring &getDirectoryPath(wstring &path)
	{
		size_t pos = path.rfind(L'\\');
		if(pos != wstring::npos)
			path.erase(++ pos);
		return path;
	}

	wstring &getFileName(wstring &path)
	{
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
