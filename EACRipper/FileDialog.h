#pragma once

#include "WindowBase.h"

namespace EACRipper
{
	class FileDialog
	{
	private:
		bool isOpen;
		WindowBase *owner;
		std::wstring filePath;
		IFileDialog *dlg;
		OPENFILENAMEW ofn;

	public:
		FileDialog(bool, WindowBase *, const std::wstring &, const std::wstring &, const std::wstring &);
		virtual ~FileDialog();

	public:
		bool show();
		const std::wstring &getPath();
	};
}
