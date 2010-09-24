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
		static uintptr_t __stdcall procCharsetFileDialog(HWND, unsigned, WPARAM, LPARAM);

	private:
		static void convertRectToClient(HWND, RECT *);

	public:
		virtual bool show();
		virtual const std::string &getCharset() const;
	};
}
