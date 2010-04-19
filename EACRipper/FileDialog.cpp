#include "Defaults.h"

#include "FileDialog.h"

using namespace std;

namespace EACRipper
{
	FileDialog::FileDialog(bool iisOpen, WindowBase *iowner, const std::wstring &ititle, const std::wstring &ifilter, const std::wstring &idefExt)
		: isOpen(iisOpen), owner(iowner), title(ititle), filter(ifilter), defExt(idefExt)
	{
	}

	FileDialog::~FileDialog()
	{
	}

	bool FileDialog::show()
	{
		return false;
	}

	const wstring &FileDialog::getPath()
	{
		return filePath;
	}
}
