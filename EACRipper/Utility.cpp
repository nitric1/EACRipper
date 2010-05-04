#include "Defaults.h"

#include "Utility.h"
#include "Singleton.h"

using namespace std;

namespace EACRipper
{
	wstring &trim(wstring &str)
	{
		wstring::iterator it;
		wstring::reverse_iterator rit;
		
		for(it = str.begin(); it != str.end(); ++ it)
		{
			if(*it != L' ' && *it != L'¡¡' && *it != L'\t' && *it != L'\n' && *it != L'\r')
				break;
		}

		str.erase(str.begin(), it);

		for(rit = str.rbegin(); rit != str.rend(); ++ rit)
		{
			if(*rit != L' ' && *rit != L'¡¡' && *rit != L'\t' && *rit != L'\n' && *rit != L'\r')
				break;
		}

		str.erase(rit.base(), str.end());

		return str;
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
