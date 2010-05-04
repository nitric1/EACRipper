#pragma once

#include "Window.h"

namespace EACRipper
{
	class FileDialog;

	class FileDialogFilter
	{
	private:
		std::vector<std::pair<std::wstring, std::wstring> > list;
		std::wstring ofnFilter;

	public:
		FileDialogFilter();
		explicit FileDialogFilter(const std::vector<std::pair<std::wstring, std::wstring> > &);
		FileDialogFilter(const FileDialogFilter &);
		virtual ~FileDialogFilter();

	public:
		virtual bool add(const std::wstring &, const std::wstring &);
		virtual bool add(const std::vector<std::pair<std::wstring, std::wstring> > &);

		virtual std::wstring getOFNFilter() const;

		friend class FileDialog;
	};

	class FileDialog
	{
	private:
		bool isOpen;
		Window *owner;
		std::wstring title, defExt, filePath;
		FileDialogFilter filter;
		std::wstring ofnFilter;
		IFileDialog *dlg;
		OPENFILENAMEW ofn;

	public:
		FileDialog(bool, Window *, const std::wstring &, const FileDialogFilter &, const std::wstring & = std::wstring());
		virtual ~FileDialog();

	public:
		virtual bool show();
		virtual const std::wstring &getPath() const;
	};
}
