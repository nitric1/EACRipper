#include "Defaults.h"

#include "MainController.h"
#include "MainWindow.h"
#include "PreferenceWindow.h"

using namespace std;

namespace EACRipper
{
	PreferenceWindow::PreferenceWindow()
		: ChildWindow(nullptr), conf(&Configure::instance())
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
					// TODO: Process Saving Preference
				}

			case IDCANCEL:
				{
					EndDialog(window, LOWORD(wParam));
					self.setWindow(nullptr);
				}
				return 1;

			case IDC_CHARSET_MARK:
				return 1;

			case IDC_CHARSET_UNMARK:
				return 1;

			case IDC_CHARSET_ORDER_UP:
				return 1;
				
			case IDC_CHARSET_ORDER_DOWN:
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

	wstring PreferenceWindow::getValue(const wstring &item)
	{
		HWND itemWin;
		if(item == L"BasePath")
			itemWin = GetDlgItem(getWindow(), IDC_PATH);
		else
			return wstring();

		int len = static_cast<size_t>(GetWindowTextLengthW(itemWin));
		vector<wchar_t> buf(static_cast<size_t>(len));
		GetWindowTextW(itemWin, &*buf.begin(), len);

		return wstring(buf.begin(), buf.end());
	}

	bool PreferenceWindow::setValue(const wstring &item, const wstring &value)
	{
		if(item == L"BasePath")
			return SetDlgItemTextW(getWindow(), IDC_PATH, value.c_str()) != FALSE;
		return false;
	}

	bool PreferenceWindow::show()
	{
		intptr_t res = DialogBoxParamW(MainController::instance().getInstance(), MAKEINTRESOURCEW(DIALOG_ID), getParent()->getWindow(), procMessage, 0);
		return res == IDOK;
	}
}
