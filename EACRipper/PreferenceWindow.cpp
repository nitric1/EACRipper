#include "Defaults.h"

#include "MainController.h"
#include "MainWindow.h"
#include "PreferenceWindow.h"
#include "Utility.h"

using namespace std;

namespace EACRipper
{
	PreferenceWindow::PreferenceWindow()
		: Dialog(nullptr), conf(&Configure::instance())
	{
	}

	PreferenceWindow::~PreferenceWindow()
	{
	}

	intptr_t __stdcall PreferenceWindow::procMessage(HWND window, unsigned message, WPARAM wParam, LPARAM lParam)
	{
		PreferenceWindow &self = instance();

		WindowEventArgs e = {&self, window, message, wParam, lParam};

		switch(message)
		{
		case WM_INITDIALOG:
			{
				self.setWindow(window);

				if(!self.runEventListener(L"prefInit", e))
					break;
			}
			return 1;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			case IDC_PATH_SELECT:
				{
					vector<wchar_t> buf(0x400);
					BROWSEINFOW bi;
					LPITEMIDLIST idl;

					GetDlgItemTextW(window, IDC_PATH, &*buf.begin(), static_cast<int>(buf.size()));

					bi.hwndOwner = window;
					bi.pidlRoot = NULL;
					bi.pszDisplayName = &*buf.begin();
					bi.lpszTitle = L"Select the target path in which music will be ripped.";
					bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_USENEWUI;
					bi.lpfn = procBrowsePath;
					bi.lParam = reinterpret_cast<LPARAM>(&*buf.begin());

					idl = SHBrowseForFolderW(&bi);

					if(idl != nullptr && SHGetPathFromIDListW(idl, &*buf.begin()))
					{
						self.setValue(L"BasePath", &*buf.begin());
					}
				}
				return 1;

			case IDC_RIP_GAP:
				if(HIWORD(wParam) == BN_CLICKED)
				{
					HWND control = reinterpret_cast<HWND>(lParam);
					bool checked = (Button_GetCheck(control) == BST_CHECKED);
					EnableWindow(GetDlgItem(window, IDC_RIP_GAP_NEXT), checked);
					EnableWindow(GetDlgItem(window, IDC_RIP_IGNORE_PREGAP_POSTGAP), checked);
				}
				return 1;

			case IDOK:
				{
					if(!self.runEventListener(L"prefOK", e))
						break;
				}

			case IDCANCEL:
				{
					self.endDialog(LOWORD(wParam));
				}
				return 1;
			}
			break;

		case WM_CLOSE:
			{
				self.endDialog(IDCANCEL);
			}
			return 1;
		}

		return 0;
	}

	int __stdcall PreferenceWindow::procBrowsePath(HWND window, unsigned message, LPARAM param, LPARAM data)
	{
		switch(message)
		{
		case BFFM_INITIALIZED:
			SendMessageW(window, BFFM_SETSELECTION, TRUE, data);
			break;
		}

		return 0;
	}

	const wchar_t *PreferenceWindow::getDialogName()
	{
		return MAKEINTRESOURCEW(DIALOG_ID);
	}

	bool PreferenceWindow::show()
	{
		return showModal(procMessage) == IDOK;
	}

	wstring PreferenceWindow::getValue(const wstring &item)
	{
		HWND itemWin;
		bool checkbox = false;
		if(item == L"BasePath")
			itemWin = GetDlgItem(getWindow(), IDC_PATH);
		else if(item == L"NameFormat")
			itemWin = GetDlgItem(getWindow(), IDC_NAME_FORMAT);
		else if(item == L"RipGap")
		{
			itemWin = GetDlgItem(getWindow(), IDC_RIP_GAP);
			checkbox = true;
		}
		else if(item == L"AttachGapNextTrack")
		{
			itemWin = GetDlgItem(getWindow(), IDC_RIP_GAP_NEXT);
			checkbox = true;
		}
		else if(item == L"IgnorePregapPostgap")
		{
			itemWin = GetDlgItem(getWindow(), IDC_RIP_IGNORE_PREGAP_POSTGAP);
			checkbox = true;
		}
		else
			return wstring();

		if(checkbox)
		{
			return (Button_GetCheck(itemWin) == BST_CHECKED) ? L"Yes" : L"No";
		}
		else
		{
			int len = static_cast<size_t>(GetWindowTextLengthW(itemWin));
			vector<wchar_t> buf(static_cast<size_t>(len) + 1);
			GetWindowTextW(itemWin, &*buf.begin(), len + 1);

			return wstring(&*buf.begin());
		}

		return wstring();
	}

	bool PreferenceWindow::setValue(const wstring &item, const wstring &value)
	{
		if(item == L"BasePath")
			return SetDlgItemTextW(getWindow(), IDC_PATH, value.c_str()) != FALSE;
		else if(item == L"NameFormat")
			return SetDlgItemTextW(getWindow(), IDC_NAME_FORMAT, value.c_str()) != FALSE;
		else if(item == L"RipGap")
		{
			HWND control = GetDlgItem(getWindow(), IDC_RIP_GAP);
			bool check = (value == L"Yes");
			Button_SetCheck(control, check ? BST_CHECKED : BST_UNCHECKED);
			EnableWindow(GetDlgItem(getWindow(), IDC_RIP_GAP_NEXT), check);
			EnableWindow(GetDlgItem(getWindow(), IDC_RIP_IGNORE_PREGAP_POSTGAP), check);
			return true;
		}
		else if(item == L"AttachGapNextTrack")
		{
			HWND control = GetDlgItem(getWindow(), IDC_RIP_GAP_NEXT);
			Button_SetCheck(control, (value == L"Yes" ? BST_CHECKED : BST_UNCHECKED));
			return true;
		}
		else if(item == L"IgnorePregapPostgap")
		{
			HWND control = GetDlgItem(getWindow(), IDC_RIP_IGNORE_PREGAP_POSTGAP);
			Button_SetCheck(control, (value == L"Yes" ? BST_CHECKED : BST_UNCHECKED));
			return true;
		}
		return false;
	}
}
