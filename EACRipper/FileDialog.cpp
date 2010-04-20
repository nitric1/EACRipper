#include "Defaults.h"

#include "FileDialog.h"
#include "Utility.h"

using namespace std;

namespace EACRipper
{
	FileDialog::FileDialog(bool iisOpen, WindowBase *iowner, const std::wstring &title, const std::wstring &filter, const std::wstring &defExt)
		: isOpen(iisOpen), owner(iowner)
	{
		// TODO: 2000/XP Implementation
		if(overOSVersion(VISTA))
		{
			HRESULT hr = CoCreateInstance(isOpen ? CLSID_FileOpenDialog : CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dlg));
			unsigned long options;
			if(FAILED(hr))
				throw(runtime_error("Failed to create a file dialog."));

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
	}

	FileDialog::~FileDialog()
	{
		dlg->Release();
	}

	bool FileDialog::show()
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
			return false;

		filePath = name;
		CoTaskMemFree(name);

		res->Release();

		return true;
	}

	const wstring &FileDialog::getPath()
	{
		return filePath;
	}
}
