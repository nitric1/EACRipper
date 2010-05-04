#include "Defaults.h"

#include "FileDialog.h"
#include "Utility.h"

using namespace std;

namespace EACRipper
{
	FileDialogFilter::FileDialogFilter()
	{
	}

	FileDialogFilter::FileDialogFilter(const vector<pair<wstring, wstring> > &ve)
		: list(ve)
	{
	}

	FileDialogFilter::FileDialogFilter(const FileDialogFilter &f)
		: list(f.list)
	{
	}

	FileDialogFilter::~FileDialogFilter()
	{
	}

	bool FileDialogFilter::add(const wstring &name, const wstring &ext)
	{
		list.push_back(make_pair(name, name));
		return true;
	}

	bool FileDialogFilter::add(const vector<pair<wstring, wstring> > &ve)
	{
		list.insert(list.end(), ve.begin(), ve.end());
		return true;
	}

	wstring FileDialogFilter::getOFNFilter() const
	{
		return wstring();
	}

	FileDialog::FileDialog(bool iisOpen, Window *iowner, const std::wstring &ititle, const FileDialogFilter &ifilter, const std::wstring &idefExt)
		: isOpen(iisOpen), owner(iowner), title(ititle), filter(ifilter), defExt(idefExt), dlg(nullptr), ofn()
	{
		HRESULT hr = CoCreateInstance(isOpen ? CLSID_FileOpenDialog : CLSID_FileSaveDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dlg));

		if(SUCCEEDED(hr) && dlg != nullptr)
		{
			unsigned long options;
			hr = dlg->GetOptions(&options);
			if(FAILED(hr))
				throw(runtime_error("Failed to get options from the file dialog."));

			hr = dlg->SetOptions(options | FOS_NOREADONLYRETURN | FOS_DONTADDTORECENT);
			if(FAILED(hr))
				throw(runtime_error("Failed to set options of the file dialog."));

			hr = dlg->SetTitle(title.c_str());
			// if(FAILED(hr)); // Set title is not much matter.

			hr = dlg->SetDefaultExtension(defExt.c_str());
			// if(FAILED(hr)); // Set default extension is not much matter also.

			// TODO: Default path
		}
		else
		{
			ofnFilter = filter.getOFNFilter();

			ofn.hwndOwner = owner->getWindow();
			ofn.lpstrTitle = title.c_str();
			ofn.lpstrDefExt = defExt.c_str();
			ofn.Flags = OFN_DONTADDTORECENT | OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
			ofn.lStructSize = sizeof(ofn);

			// TODO: Filter
		}
	}

	FileDialog::~FileDialog()
	{
		if(dlg != nullptr)
			dlg->Release();
	}

	bool FileDialog::show()
	{
		if(dlg != nullptr)
		{
			IShellItem *res;
			wchar_t *name;
			HRESULT hr = dlg->Show(owner->getWindow());
			if(FAILED(hr))
				return false;

			hr = dlg->GetResult(&res);
			if(FAILED(hr))
				return false;

			res->GetDisplayName(SIGDN_FILESYSPATH, &name);
			if(FAILED(hr))
			{
				CoTaskMemFree(name);
				return false;
			}

			filePath = name;
			CoTaskMemFree(name);

			res->Release();
		}
		else // 2000/XP
		{
			vector<wchar_t> buf(0x1000);

			ofn.lpstrFile = &*buf.begin();
			ofn.nMaxFile = static_cast<unsigned long>(buf.size());

			if(!GetOpenFileNameW(&ofn))
				return false;

			filePath = &*buf.begin();
		}

		return true;
	}

	const wstring &FileDialog::getPath() const
	{
		return filePath;
	}
}
