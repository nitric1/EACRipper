#include "Defaults.h"

#include "MainController.h"
#include "AboutWindow.h"
#include "Info.h"

using namespace std;

namespace EACRipper
{
	AboutWindow::AboutWindow()
		: mouseOver(false)
	{
	}

	AboutWindow::~AboutWindow()
	{
	}

	intptr_t AboutWindow::procMessageImpl(HWND window, uint32_t message, uintptr_t wParam, longptr_t lParam)
	{
		switch(message)
		{
		case WM_INITDIALOG:
			{
				setWindow(window);

				SetDlgItemTextW(window, IDC_EACRIPPER_TEXT, Information::fullName);

				HWND link = GetDlgItem(window, IDC_WEBSITE_LINK);

				linkOldProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(link,
					GWLP_WNDPROC, reinterpret_cast<intptr_t>(procLink)));
				mouseOver = false;
			}
			return 1;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			case IDOK:
			case IDCANCEL:
				{
					endDialog(LOWORD(wParam));
				}
				return 1;
			}
			break;

		case WM_CLOSE:
			{
				endDialog(IDOK);
			}
			return 1;
		}

		return 0;
	}

	uintptr_t AboutWindow::procLinkImpl(HWND window, uint32_t message, uintptr_t wParam, longptr_t lParam)
	{
		switch(message)
		{
		case WM_SETCURSOR:
			{
				if(mouseOver)
					SetCursor(LoadCursorW(NULL, IDC_HAND));
			}
			return 0;

		case WM_MOUSEMOVE:
			{
				if(!mouseOver)
				{
					mouseOver = true;

					TRACKMOUSEEVENT tme;
					tme.cbSize = sizeof(tme);
					tme.dwFlags = TME_LEAVE;
					tme.hwndTrack = window;
					tme.dwHoverTime = HOVER_DEFAULT;
					TrackMouseEvent(&tme);

					InvalidateRect(window, NULL, TRUE);
				}
			}
			return 0;

		case WM_MOUSELEAVE:
			{
				mouseOver = false;

				InvalidateRect(window, NULL, TRUE);
			}
			return 0;

		case WM_LBUTTONUP:
			{
				int len = GetWindowTextLengthW(window);
				vector<wchar_t> ve(static_cast<size_t>(len + 1));
				GetWindowTextW(window, &*ve.begin(), len + 1);

				ShellExecuteW(getParent()->getWindow(), L"open", &*ve.begin(), NULL, NULL, SW_SHOW);
			}
			return 0;

		case WM_NCHITTEST:
			{
				return HTCLIENT;
			}

		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(window, &ps);
				HFONT winFont = GetWindowFont(window), newFont;
				HGDIOBJ oldFont;
				COLORREF oldColor;

				RECT rc;
				GetClientRect(window, &rc);

				FillRect(hdc, &rc, GetSysColorBrush(COLOR_BTNFACE));

				LOGFONTW lf;
				GetObjectW(winFont, sizeof(lf), &lf);
				lf.lfUnderline = 1;
				newFont = CreateFontIndirectW(&lf);
				oldFont = SelectObject(hdc, newFont);

				int len = GetWindowTextLengthW(window);
				vector<wchar_t> ve(static_cast<size_t>(len + 1));
				GetWindowTextW(window, &*ve.begin(), len + 1);

				SetBkMode(hdc, TRANSPARENT);

				if(mouseOver)
					oldColor = SetTextColor(hdc, RGB(0xFF, 0, 0));
				else
					oldColor = SetTextColor(hdc, RGB(0, 0, 0xFC));

				TextOutW(hdc, 0, 0, &*ve.begin(), len);

				SetTextColor(hdc, oldColor);

				SelectObject(hdc, oldFont);
				DeleteObject(newFont);
				EndPaint(window, &ps);
			}
			return 0;
		}

		return CallWindowProcW(linkOldProc, window, message, wParam, lParam);
	}

	intptr_t __stdcall AboutWindow::procMessage(HWND window, uint32_t message, uintptr_t wParam, longptr_t lParam)
	{
		return instance().procMessageImpl(window, message, wParam, lParam);
	}

	uintptr_t __stdcall AboutWindow::procLink(HWND window, uint32_t message, uintptr_t wParam, longptr_t lParam)
	{
		return instance().procLinkImpl(window, message, wParam, lParam);
	}

	const wchar_t *AboutWindow::getDialogName()
	{
		return MAKEINTRESOURCEW(DIALOG_ID);
	}

	bool AboutWindow::show()
	{
		showModal(procMessage);
		return true;
	}
}
