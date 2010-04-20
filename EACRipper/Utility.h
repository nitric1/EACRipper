#pragma once

namespace EACRipper
{
	typedef enum _OSVERSION
	{
		ALL = 0,
		NT2000 = 1,
		XP = 2,
		SERVER2003 = 3,
		VISTA = 4,
		SERVER2008 = 5,
		SEVEN = 6,
		SERVER2008R2 = 7,
	} OSVERSION;

	std::wstring &trim(std::wstring &);
	std::wstring &getDirectoryPath(std::wstring &);
	std::wstring &getFileName(std::wstring &);
	std::wstring getCurrentDirectoryPath();
	bool overOSVersion(OSVERSION);
}
