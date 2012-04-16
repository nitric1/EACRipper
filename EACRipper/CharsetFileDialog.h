#pragma once

#include "FileDialog.h"

namespace EACRipper
{
	class CharsetFileDialog : public FileDialog
	{
	private:
		std::string charset;
		std::vector<std::wstring> charsetList;
		size_t charsetIdx;

	public:
		CharsetFileDialog(bool, Window *, const std::wstring &, const FileDialogFilter &, const std::wstring & = std::wstring());
		virtual ~CharsetFileDialog();

	private:
		uintptr_t procCharsetFileDialogImpl(HWND, uint32_t, uintptr_t, longptr_t);

		static uintptr_t __stdcall procCharsetFileDialog(HWND, uint32_t, uintptr_t, longptr_t);

	private:
		static void convertRectToClient(HWND, RECT *);

	public:
		virtual bool show();
		virtual const std::string &getCharset() const;
	};
}
