#include "Defaults.h"

#include "MainController.h"
#include "MainWindow.h"

using namespace std;

namespace EACRipper
{
	MainWindow::MainWindow()
		: Window(nullptr), shortcut(&ShortcutKey::instance())
	{
	}

	MainWindow::~MainWindow()
	{
	}

	intptr_t __stdcall MainWindow::procMessage(HWND window, unsigned message, WPARAM wParam, LPARAM lParam)
	{
		MainWindow &self = instance();

		WindowEventArgs e = {&self, window, message, wParam, lParam};

		switch(message)
		{
		case WM_INITDIALOG:
			{
				self.setWindow(window);

				self.iconSmall = static_cast<HICON>(LoadImageW(MainController::instance().getInstance(), MAKEINTRESOURCEW(IDI_MAIN_ICON), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR));
				self.iconBig = static_cast<HICON>(LoadImageW(MainController::instance().getInstance(), MAKEINTRESOURCEW(IDI_MAIN_ICON), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR));

				SendMessageW(window, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(self.iconSmall));
				SendMessageW(window, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(self.iconBig));

				HWND list = GetDlgItem(window, IDC_LIST);

				ListView_SetExtendedListViewStyle(list, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

				SetWindowTextW(window, EACRIPPER_TITLE L" " EACRIPPER_VERSION);

				if(!self.runEventListener(L"init", e))
					break;
			}
			return 1;

		case WM_KEYDOWN:
			if(self.shortcut->processKeydownMessage(&self, wParam, lParam))
			{
				return 1;
			}
			break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			case IDM_FILE_OPEN:
				if(!self.runEventListener(L"openCuesheet", e))
					break;
				return 1;

			case IDM_INCUE_OPEN:
				if(!self->runEventListener(L"openInCue", e))
					break;
				return 1;

			case IDM_ARCHIVE_OPEN:
				if(!self.runEventListener(L"openArchive", e))
					break;
				return 1;

			case IDM_OPTION:
				if(!self.runEventListener(L"option", e))
					break;
				return 1;

			case IDM_RIP:
				if(!self.runEventListener(L"rip", e))
					break;
				return 1;

			case IDC_FORMAT:
				if(HIWORD(wParam) == CBN_SELCHANGE)
				{
					return 1;
				}
				break;

			case IDC_FORMAT_SETTING:
				return 1;

			case IDC_ALBUM_ARTIST:
				if(HIWORD(wParam) == EN_CHANGE)
				{
					return 1;
				}
				break;

			case IDC_ARTIST:
				if(HIWORD(wParam) == EN_CHANGE)
				{
					return 1;
				}
				break;

			case IDC_TITLE:
				if(HIWORD(wParam) == EN_CHANGE)
				{
					return 1;
				}
				break;

			case IDC_SET_COVER_ART:
				if(!self.runEventListener(L"setCoverArt", e))
					break;
				return 1;

			case IDC_CANCEL_COVER_ART:
				if(!self.runEventListener(L"cancelCoverArt", e))
					break;
				return 1;

			case IDC_CANCEL:
				// TODO: Is ripping
				{
					return 1;
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
							return 1;
						}
					}
					break;
				}
			}
			break;

		case WM_CLOSE:
			{
				if(!self.runEventListener(L"close", e))
					break;

				EndDialog(window, 0);

				DestroyIcon(self.iconSmall);
				DestroyIcon(self.iconBig);

				self.setWindow(nullptr);
			}
			return 1;
		}

		return 0;
	}

	bool MainWindow::show()
	{
		intptr_t res = DialogBoxParamW(MainController::instance().getInstance(), MAKEINTRESOURCEW(DIALOG_ID), nullptr, procMessage, 0);
		return res == IDOK;
	}

	void MainWindow::addFormat(const wstring &name)
	{
		HWND formatList = GetDlgItem(getWindow(), IDC_FORMAT);

		ComboBox_AddString(formatList, name.c_str());
	}

	void MainWindow::selectFormat(const wstring &name)
	{
		HWND formatList = GetDlgItem(getWindow(), IDC_FORMAT);
		int idx = ComboBox_FindStringExact(formatList, -1, name.c_str());
		if(idx == CB_ERR)
			return;
		ComboBox_SetCurSel(formatList, idx);
	}
}
