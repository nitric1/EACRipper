#include "Defaults.h"

#include "MainController.h"
#include "MainWindow.h"

using namespace std;
using namespace Gdiplus;

namespace EACRipper
{
	MainWindow::MainWindow()
		: Window(nullptr), iconSmall(nullptr), iconBig(nullptr), shortcut(&ShortcutKey::instance()), baseUnitX(1), baseUnitY(1)
	{
	}

	MainWindow::~MainWindow()
	{
		coverArtThumbnail = nullptr;
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

				self.initList();

				SetWindowTextW(window, EACRIPPER_TITLE L" " EACRIPPER_VERSION);

				self.coverArtOldProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(GetDlgItem(window, IDC_COVER_ART), GWLP_WNDPROC, reinterpret_cast<intptr_t>(procCoverArt)));

				RECT rc = {0, 0, 4, 8};
				MapDialogRect(window, &rc);

				self.baseUnitX = rc.right;
				self.baseUnitY = rc.bottom;

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

		case WM_GETMINMAXINFO:
			{
				MINMAXINFO *mmi = reinterpret_cast<MINMAXINFO *>(lParam);
				mmi->ptMinTrackSize.x = 730;
				mmi->ptMinTrackSize.y = 525;
			}
			return 1;

		case WM_SIZE:
			{
				self.resizeLayout(LOWORD(lParam), HIWORD(lParam));
			}
			return 1;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			case IDM_FILE_OPEN:
				if(!self.runEventListener(L"openCuesheet", e))
					break;
				return 1;

			case IDM_INCUE_OPEN:
				if(!self.runEventListener(L"openInCue", e))
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

			case IDM_ABOUT:
				if(!self.runEventListener(L"about", e))
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
						/*switch(hdr->code)
						{
						}*/
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

	uintptr_t __stdcall MainWindow::procCoverArt(HWND window, unsigned message, WPARAM wParam, LPARAM lParam)
	{
		MainWindow &self = instance();

		switch(message)
		{
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(window, &ps);
				RECT rc;

				GetClientRect(window, &rc);
				++ rc.left; ++ rc.top;
				-- rc.right; -- rc.bottom;

				FillRect(hdc, &rc, GetSysColorBrush(COLOR_BTNFACE));

				if(self.coverArtThumbnail != nullptr)
				{
					Graphics g(hdc);
					g.DrawImage(self.coverArtThumbnail, self.coverArtLeft, self.coverArtTop, self.coverArtWidth, self.coverArtHeight);
				}

				EndPaint(window, &ps);
			}
			return 0;
		}

		return CallWindowProcW(self.coverArtOldProc, window, message, wParam, lParam);
	}

	bool MainWindow::show()
	{
		intptr_t res = DialogBoxParamW(MainController::instance().getInstance(), MAKEINTRESOURCEW(DIALOG_ID), nullptr, procMessage, 0);
		return res == IDOK;
	}

	void MainWindow::resizeLayout(int width, int height)
	{
		int dlgX, dlgY;

		dlgX = width * 4 / baseUnitX;
		dlgY = height * 8 / baseUnitY;

		resizeItemDLU(IDC_GROUP_ALBUM_INFO, 5, 5, dlgX - 100, 121);
		resizeItemDLU(IDC_ALBUM_TITLE, 60, 17, dlgX - 104, 31);
		resizeItemDLU(IDC_ALBUM_ARTIST, 60, 35, dlgX - 104, 49);
		resizeItemDLU(IDC_FORMAT, 60, 104, dlgX - 158, 116);
		resizeItemDLU(IDC_FORMAT_SETTING, dlgX - 154, 103, dlgX - 104, 117);

		resizeItemDLU(IDC_GROUP_COVER_ART, dlgX - 95, 5, dlgX - 5, 121);
		resizeItemDLU(IDC_COVER_ART, dlgX - 91, 17, dlgX - 9, 99);
		resizeItemDLU(IDC_SET_COVER_ART, dlgX - 91, 103, dlgX - 52, 117);
		resizeItemDLU(IDC_CANCEL_COVER_ART, dlgX - 48, 103, dlgX - 9, 117);

		resizeItemDLU(IDC_LIST, 5, 126, dlgX - 5, dlgY - 24);

		resizeItemDLU(IDC_PROGRESS_TEXT, 5, dlgY - 16, 75, dlgY - 8);
		resizeItemDLU(IDC_PROGRESS, 79, dlgY - 19, dlgX - 59, dlgY - 5);
		resizeItemDLU(IDC_CANCEL, dlgX - 55, dlgY - 19, dlgX - 5, dlgY - 5);

		InvalidateRect(getWindow(), NULL, TRUE);
	}

	void MainWindow::resizeItemDLU(int id, int left, int top, int right, int bottom)
	{
		RECT rc = {left, top, right, bottom};
		MapDialogRect(getWindow(), &rc);

		MoveWindow(GetDlgItem(getWindow(), id), rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, FALSE);
	}

	void MainWindow::initList()
	{
		HWND list = GetDlgItem(getWindow(), IDC_LIST);
		ListView_SetExtendedListViewStyle(list, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

		LVCOLUMNW lvc;
		lvc.cx = 50;
		lvc.fmt = LVCFMT_RIGHT;
		lvc.pszText = L"#";
		lvc.mask = LVCF_WIDTH | LVCF_FMT | LVCF_TEXT;
		ListView_InsertColumn(list, 0, &lvc);

		lvc.cx = 120;
		lvc.fmt = LVCFMT_LEFT;
		lvc.pszText = L"Artist";
		ListView_InsertColumn(list, 1, &lvc);

		lvc.cx = 210;
		lvc.fmt = LVCFMT_LEFT;
		lvc.pszText = L"Title";
		ListView_InsertColumn(list, 2, &lvc);

		lvc.cx = 100;
		lvc.fmt = LVCFMT_LEFT;
		lvc.pszText = L"Composer";
		ListView_InsertColumn(list, 3, &lvc);

		lvc.cx = 175;
		lvc.fmt = LVCFMT_LEFT;
		lvc.pszText = L"Length";
		ListView_InsertColumn(list, 4, &lvc);

		lvc.cx = 60;
		lvc.fmt = LVCFMT_LEFT;
		lvc.pszText = L"Process";
		ListView_InsertColumn(list, 5, &lvc);

		/*LVITEMW lvi = LVITEMW();
		wchar_t *lstr[][19] =
		{
			{L"右肩の蝶", L"初音ミクの暴走 -Full ver.-", L"いろは唄", L"ローリンガール", L"パラジクロロベンゼン", L"IMITATION BLACK", L"イケ恋歌", L"番凩", L"紡唄 -つむぎうた-",
			 L"リグレットメッセージ", L"しねばいいのに", L"リア充爆発しろ!", L"charActer", L"1/6 -genesis mix-", L"erase or zero (Vocalogenesis only NewMix)", L"Nostalogic (single edit)",
			 L"DYE", L"Genesis", L"右肩の蝶 -Ver. 3.0.1-"},
			{L"鏡音レン", L"初音ミク", L"鏡音リン", L"初音ミク", L"鏡音レン", L"KAITO&鏡音レン&神威がくぽ", L"鏡音レン", L"MEIKO&KAITO", L"鏡音リン&鏡音レン",
			 L"鏡音リン", L"KAITO", L"初音ミク", L"初音ミク&巡音ルカ", L"初音ミク", L"KAITO&鏡音レン", L"MEIKO", L"巡音ルカ", L"初音ミク&巡音ルカ", L"鏡音レン"},
			{L"のりぴー", L"cosMo@暴走P", L"銀サク", L"現実逃避P/wowaka", L"オワタP", L"SCL Project(natsuP)", L"れれれP", L"仕事してP", L"DATEKEN",
			 L"悪ノP/mothy", L"どぶウサギ", L"KAZU-k", L"azuma", L"ぼーかりおどP/noa", L"クリスタルP/HzEdge", L"yuukiss", L"AVTechNO!", L"ジミーサムP", L"のりぴー"},
			{L"04:24:60 (00:00:00-04:24:60)", 
			L"04:47:70 (04:24:60-09:12:30)", 
			L"04:05:92 (09:12:30-13:18:22)", 
			L"03:12:00 (13:18:22-16:30:22)", 
			L"03:54:92 (16:30:22-20:25:14)", 
			L"03:43:45 (20:25:14-24:08:59)", 
			L"03:41:00 (24:08:59-27:49:59)", 
			L"03:45:07 (27:49:59-31:34:66)", 
			L"05:21:92 (31:34:66-36:56:58)", 
			L"03:22:62 (36:56:58-40:19:20)", 
			L"05:31:85 (40:19:20-45:51:05)", 
			L"01:33:45 (45:51:05-47:24:50)", 
			L"03:53:22 (47:24:50-51:17:72)", 
			L"04:20:53 (51:17:72-55:38:25)", 
			L"03:50:92 (55:38:49-59:29:41)", 
			L"04:00:77 (59:29:41-63:30:18)", 
			L"03:49:30 (63:30:18-67:19:48)", 
			L"05:39:77 (67:19:48-72:59:25)", 
			L"04:55:68 (72:59:25-77:54:93)"}
		};
		wchar_t buf[10];

		lvi.mask = LVIF_TEXT;

		for(int i = 0; i < 19; ++ i)
		{
			_itow_s(i + 1, buf, 10);

			lvi.iItem = i;
			lvi.iSubItem = 0;
			lvi.pszText = buf;
			ListView_InsertItem(list, &lvi);

			lvi.iSubItem = 1;
			lvi.pszText = lstr[1][i];
			ListView_SetItem(list, &lvi);

			lvi.iSubItem = 2;
			lvi.pszText = lstr[0][i];
			ListView_SetItem(list, &lvi);

			lvi.iSubItem = 3;
			lvi.pszText = lstr[2][i];
			ListView_SetItem(list, &lvi);

			lvi.iSubItem = 4;
			lvi.pszText = lstr[3][i];
			ListView_SetItem(list, &lvi);

			ListView_SetCheckState(list, lvi.iItem, TRUE);
		}*/
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

	void MainWindow::setCoverArt(const wstring &path)
	{
		HWND coverArt = GetDlgItem(getWindow(), IDC_COVER_ART);
		RECT rc;

		if(coverArtThumbnail != nullptr)
		{
			delete coverArtThumbnail;
			coverArtThumbnail = nullptr;
		}

		GetClientRect(coverArt, &rc);
		++ rc.left; ++ rc.top;
		-- rc.right; -- rc.bottom;

		Image img(path.c_str());

		unsigned iw = img.GetWidth(), ih = img.GetHeight();

		if(iw > ih)
		{
			coverArtWidth = max<int>(1, rc.right - rc.left - 1);
			coverArtHeight = max<int>(1, static_cast<int>(ih * static_cast<double>(coverArtWidth) / iw) - 1);

			coverArtTop = 1 + (rc.bottom - rc.top - coverArtHeight) / 2;
			coverArtLeft = 1;
		}
		else
		{
			coverArtHeight = max<int>(1, rc.bottom - rc.top);
			coverArtWidth = max<int>(1, static_cast<int>(iw * static_cast<double>(coverArtHeight) / ih) - 1);

			if(coverArtWidth <= 0)
				coverArtWidth = 1;

			coverArtTop = 1;
			coverArtLeft = 1 + (rc.right - rc.left - coverArtWidth) / 2;
		}

		coverArtThumbnail = img.GetThumbnailImage(static_cast<unsigned>(coverArtWidth), static_cast<unsigned>(coverArtHeight));
		
		InvalidateRect(coverArt, NULL, TRUE);
	}

	void MainWindow::cancelCoverArt()
	{
		if(coverArtThumbnail != nullptr)
		{
			delete coverArtThumbnail;
			coverArtThumbnail = nullptr;
		}

		InvalidateRect(GetDlgItem(getWindow(), IDC_COVER_ART), NULL, TRUE);
	}
}
