#include "Defaults.h"

#include "FileDialog.h"
#include "Utility.h"

using namespace std;

namespace EACRipper
{
	FileDialogFilter::FileDialogFilter()
	{
	}

	FileDialogFilter::FileDialogFilter(const vector<pair<wstring, wstring>> &ve)
		: list(ve)
	{
	}

	FileDialogFilter::FileDialogFilter(vector<pair<wstring, wstring>> &&ve)
		: list(ve)
	{
	}

	FileDialogFilter::FileDialogFilter(const FileDialogFilter &f)
		: list(f.list)
	{
	}

	FileDialogFilter::FileDialogFilter(FileDialogFilter &&f)
		: list(move(f.list))
	{
	}

	FileDialogFilter::~FileDialogFilter()
	{
	}

	bool FileDialogFilter::add(const wstring &name, const wstring &ext)
	{
		list.push_back(make_pair(name, ext));
		return true;
	}

	bool FileDialogFilter::add(const vector<pair<wstring, wstring> > &ve)
	{
		list.insert(list.end(), ve.begin(), ve.end());
		return true;
	}

	vector<COMDLG_FILTERSPEC> FileDialogFilter::getCDFilter() const
	{
		vector<COMDLG_FILTERSPEC> cdFilter;
		COMDLG_FILTERSPEC cdf;

		for(auto it = list.begin(); it != list.end(); ++ it)
		{
			cdf.pszName = it->first.c_str();
			cdf.pszSpec = it->second.c_str();
			cdFilter.push_back(cdf);
		}

		return cdFilter;
	}

	wstring FileDialogFilter::getOFNFilter() const
	{
		wstring ofnFilter;

		for(auto it = list.begin(); it != list.end(); ++ it)
		{
			ofnFilter += it->first;
			ofnFilter += L" (";
			ofnFilter += it->second;
			ofnFilter += L")";
			ofnFilter += L'\0';
			ofnFilter += it->second;
			ofnFilter += L'\0';
		}

		ofnFilter.append(3, L'\0');

		return ofnFilter;
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

			dlg->SetTitle(title.c_str());
			dlg->SetDefaultExtension(defExt.c_str());

			const vector<COMDLG_FILTERSPEC> &vcf = filter.getCDFilter();

			if(!vcf.empty())
			{
				hr = dlg->SetFileTypes(static_cast<unsigned>(vcf.size()), &*vcf.begin());
				if(FAILED(hr))
					throw(runtime_error("Failed to set file filters."));
			}

			/*hr = dlg->QueryInterface(IID_PPV_ARGS(&cust));

			cust->StartVisualGroup(0, L"&Encoding");

			cust->EndVisualGroup();*/
		}
		else
		{
			ofnFilter = filter.getOFNFilter();

			ofn.hwndOwner = owner->getWindow();
			ofn.lpstrTitle = title.c_str();
			ofn.lpstrDefExt = defExt.c_str();
			ofn.lpstrFilter = ofnFilter.c_str();
			ofn.Flags = OFN_DONTADDTORECENT | OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
			ofn.lStructSize = sizeof(ofn);
		}
	}

	FileDialog::~FileDialog()
	{
		/*if(cust != nullptr)
			cust->Release();*/
		if(dlg != nullptr)
			dlg->Release();
	}

	bool FileDialog::show()
	{
		if(dlg != nullptr)
		{
			IShellItem *res;
			wchar_t *name = nullptr;
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
