#include "Defaults.h"

#include "MainController.h"
#include "MainWindow.h"

namespace EACRipper
{
	MainWindow::MainWindow()
		: Window()
		, window(NULL)
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
				HWND list;

				self->window = window;

				self->iconSmall = static_cast<HICON>(LoadImageW(MainController::instance().getInstance(), MAKEINTRESOURCEW(IDI_MAIN_ICON), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR));
				self->iconBig = static_cast<HICON>(LoadImageW(MainController::instance().getInstance(), MAKEINTRESOURCEW(IDI_MAIN_ICON), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR));

				SendMessageW(window, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(self->iconSmall));
				SendMessageW(window, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(self->iconBig));

				list = GetDlgItem(window, IDC_LIST);

				ListView_SetExtendedListViewStyle(list, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

				if(!self->runEventListener(L"init", e))
					break;
			}
			return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			case IDM_FILE_OPEN:
				return TRUE;

			case IDM_ARCHIVE_OPEN:
				return TRUE;

			case IDM_OPTION:
				return TRUE;

			case IDM_RIP:
				return TRUE;

			case IDC_FORMAT:
				if(HIWORD(wParam) == CBN_SELCHANGE)
				{
					return TRUE;
				}
				break;

			case IDC_FORMAT_SETTING:
				return TRUE;

			case IDC_ALBUM_ARTIST:
				if(HIWORD(wParam) == EN_CHANGE)
				{
					return TRUE;
				}
				break;

			case IDC_ARTIST:
				if(HIWORD(wParam) == EN_CHANGE)
				{
					return TRUE;
				}
				break;

			case IDC_TITLE:
				if(HIWORD(wParam) == EN_CHANGE)
				{
					return TRUE;
				}
				break;

			case IDC_SET_COVER_ART:
				{
				}
				return TRUE;

			case IDC_CANCEL_COVER_ART:
				{
				}
				return TRUE;

			case IDC_CANCEL:
				// TODO: Is ripping
				{
					return TRUE;
				}
				break;
			}
			break;

		case WM_NOTIFY:
			{
				NMHDR *hdr = reinterpret_cast<NMHDR *>(lParam);
				switch(hdr->idFrom)
				{
				case IDC_LIST:
					{
						switch(hdr->code)
						{
						case LVN_ITEMCHANGED:
							return TRUE;
						}
					}
					break;
				}
			}
			break;

		case WM_CLOSE:
			{
				if(!self->runEventListener(L"close", e))
					break;

				EndDialog(window, 0);

				DestroyIcon(self->iconSmall);
				DestroyIcon(self->iconBig);

				self->window = NULL;
			}
			return TRUE;
		}

		return FALSE;
	}

	bool MainWindow::show()
	{
		return DialogBoxParamW(MainController::instance().getInstance(), MAKEINTRESOURCEW(DIALOG_ID), NULL, procMessage, NULL) == IDOK;
	}
}
