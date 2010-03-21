#include "Defaults.h"

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
}
