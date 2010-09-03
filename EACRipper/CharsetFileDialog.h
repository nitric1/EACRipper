#pragma once

#include "FileDialog.h"

namespace EACRipper
{
	class CharsetFileDialog : public FileDialog
	{
	public:
		CharsetFileDialog(bool, Window *, const std::wstring &, const FileDialogFilter &, const std::wstring & = std::wstring());
		virtual ~CharsetFileDialog();

	public:
		virtual bool show();
	};
}
