#include "Defaults.h"

#include "MainController.h"
#include "MainWindow.h"

namespace EACRipper
{
	MainWindow::MainWindow()
		: Window()
	{
	}

	MainWindow::~MainWindow()
	{
	}

	HWND MainWindow::getWindow()
	{
		return window;
	}

	BOOL __stdcall MainWindow::procMessage(HWND window, unsigned message, WPARAM wParam, LPARAM lParam)
	{
		WindowEventArgs e = {window, message, wParam, lParam};

		MainWindow *self = &instance();

		switch(message)
		{
		case WM_INITDIALOG:
			{
				self->window = window;
			}
			return TRUE;

		case WM_CLOSE:
			{
				if(!self->runEventListener("close", e))
					break;

				EndDialog(window, 0);
			}
			return TRUE;
		}

		return FALSE;
	}

	bool MainWindow::show()
	{
		return DialogBoxParamW(getOwner()->getInstance(), MAKEINTRESOURCEW(IDD_MAIN_DIALOG), NULL, procMessage, NULL) == IDOK;
	}
}
