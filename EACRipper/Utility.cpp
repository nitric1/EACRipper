#include "Defaults.h"

#include <vector>

#include "Utility.h"

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

		pathlen = GetModuleFileNameW(NULL, &*buffer.begin(), static_cast<DWORD>(buffer.size()));

		wstring path(buffer.begin(), buffer.begin() + pathlen);
		return getDirectoryPath(path);
	}
}
