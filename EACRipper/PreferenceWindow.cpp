#include "Defaults.h"

#include "MainController.h"
#include "MainWindow.h"
#include "PreferenceWindow.h"

using namespace std;

namespace EACRipper
{
	PreferenceWindow::PreferenceWindow()
		: Window(nullptr), conf(&Configure::instance())
	{
	}

	PreferenceWindow::~PreferenceWindow()
	{
	}

	intptr_t __stdcall PreferenceWindow::procMessage(HWND window, unsigned message, WPARAM wParam, LPARAM lParam)
	{
		WindowEventArgs e = {window, message, wParam, lParam};

		PreferenceWindow &self = instance();

		switch(message)
		{
		case WM_INITDIALOG:
			{
				self.window = window;

				// TODO: Preference Initialization (e.g. Fill path value, etc...)
				break;
			}
			return 1;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			case IDOK:
				{
					// TODO: Process Saving Preference
					EndDialog(window, IDOK);
					self.window = nullptr;
				}
				return 1;

			case IDCANCEL:
				{
					EndDialog(window, IDCANCEL);
					self.window = nullptr;
				}
				return 1;
			}
			break;

		case WM_CLOSE:
			{
				EndDialog(window, IDCANCEL);
				self.window = nullptr;
			}
			return 1;
		}

		return 0;
	}

	bool PreferenceWindow::show()
	{
		intptr_t res = DialogBoxParamW(MainController::instance().getInstance(), MAKEINTRESOURCEW(DIALOG_ID), nullptr, procMessage, 0);
		return res == IDOK;
	}
}
