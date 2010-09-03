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
			case IDOK:
				{
					if(!self.runEventListener(L"prefOK", e))
						break;
				}

			case IDCANCEL:
				{
					EndDialog(window, LOWORD(wParam));
					self.setWindow(nullptr);
				}
				return 1;
			}
			break;

		case WM_CLOSE:
			{
				EndDialog(window, IDCANCEL);
				self.setWindow(nullptr);
			}
			return 1;
		}

		return 0;
	}

	const wchar_t *PreferenceWindow::getDialogName()
	{
		return MAKEINTRESOURCEW(DIALOG_ID);
	}

	bool PreferenceWindow::show()
	{
		return Dialog::show(procMessage);
	}

	wstring PreferenceWindow::getValue(const wstring &item)
	{
		HWND itemWin;
		if(item == L"BasePath")
			itemWin = GetDlgItem(getWindow(), IDC_PATH);
		else if(item == L"NameFormat")
			itemWin = GetDlgItem(getWindow(), IDC_NAME_FORMAT);
		else
			return wstring();

		int len = static_cast<size_t>(GetWindowTextLengthW(itemWin));
		vector<wchar_t> buf(static_cast<size_t>(len) + 1);
		GetWindowTextW(itemWin, &*buf.begin(), len + 1);

		return wstring(&*buf.begin());
	}

	bool PreferenceWindow::setValue(const wstring &item, const wstring &value)
	{
		if(item == L"BasePath")
			return SetDlgItemTextW(getWindow(), IDC_PATH, value.c_str()) != FALSE;
		else if(item == L"NameFormat")
			return SetDlgItemTextW(getWindow(), IDC_NAME_FORMAT, value.c_str()) != FALSE;
		return false;
	}
}
