#include "Defaults.h"

#include "MainController.h"
#include "MainWindow.h"

using namespace std;
using namespace Gdiplus;

namespace EACRipper
{
	// TODO: Implementaion of to load/save the window size and list columns' size.

	MainWindow::MainWindow()
		: Dialog(nullptr), iconSmall(nullptr), iconBig(nullptr), shortcut(&ShortcutKey::instance()), baseUnitX(1), baseUnitY(1)
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

				self.trackList.attach(GetDlgItem(window, IDC_LIST));

				self.initList();

				SetWindowTextW(window, EACRIPPER_TITLE L" " EACRIPPER_VERSION);

				self.coverArtOldProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(GetDlgItem(window, IDC_COVER_ART), GWLP_WNDPROC, reinterpret_cast<intptr_t>(procCoverArt)));

				RECT rc = {0, 0, 4, 8};
				MapDialogRect(window, &rc);

				self.baseUnitX = rc.right;
				self.baseUnitY = rc.bottom;

				ShowWindow(window, self.getShowStatus());

				GetClientRect(window, &rc);

				self.resizeLayout(rc.right - rc.left, rc.bottom - rc.top);

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
				mmi->ptMinTrackSize.x = 630;
				mmi->ptMinTrackSize.y = 440;
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
				if(false)
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

				self.uninit();

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

	const wchar_t *MainWindow::getDialogName()
	{
		return MAKEINTRESOURCEW(DIALOG_ID);
	}

	bool MainWindow::show()
	{
		return Dialog::show(procMessage);
	}

	void MainWindow::resizeLayout(int width, int height)
	{
		int dlgX, dlgY;

		dlgX = width * 4 / baseUnitX;
		dlgY = height * 8 / baseUnitY;

		resizeItemDLU(IDC_GROUP_ALBUM_INFO, 5, 5, dlgX - 100, 121);
		resizeItemDLU(IDC_ALBUM_TITLE, 70, 17, dlgX - 104, 31);
		resizeItemDLU(IDC_ALBUM_ARTIST, 70, 35, dlgX - 104, 49);
		resizeItemDLU(IDC_FORMAT, 70, 104, dlgX - 168, 116);
		resizeItemDLU(IDC_FORMAT_SETTING, dlgX - 164, 103, dlgX - 104, 117);

		resizeItemDLU(IDC_GROUP_COVER_ART, dlgX - 95, 5, dlgX - 5, 121);
		resizeItemDLU(IDC_COVER_ART, dlgX - 91, 17, dlgX - 9, 99);
		resizeItemDLU(IDC_SET_COVER_ART, dlgX - 91, 103, dlgX - 52, 117);
		resizeItemDLU(IDC_CANCEL_COVER_ART, dlgX - 48, 103, dlgX - 9, 117);

		resizeItemDLU(IDC_LIST, 5, 126, dlgX - 5, dlgY - 24);

		resizeItemDLU(IDC_PROGRESS_TEXT, 5, dlgY - 16, 85, dlgY - 8);
		resizeItemDLU(IDC_PROGRESS, 89, dlgY - 19, dlgX - 59, dlgY - 5);
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

		trackList.addColumn(L"#", 50, EditListControl::ALIGN_RIGHT);
		trackList.addColumn(L"Artist", 120, EditListControl::ALIGN_LEFT);
		trackList.addColumn(L"Title", 210, EditListControl::ALIGN_LEFT);
		trackList.addColumn(L"Composer", 100, EditListControl::ALIGN_LEFT);
		trackList.addColumn(L"Length (M:S:MS)", 180, EditListControl::ALIGN_LEFT);
		trackList.addColumn(L"Progress", 60, EditListControl::ALIGN_RIGHT);
	}

	void MainWindow::uninit()
	{
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

	void MainWindow::setTrackList(const TrackList &list)
	{
		static wstring fields[] =
		{
			L"Track",
			L"Artist",
			L"Title",
			L"Composer",
			L"Length"
		};

		clearTrackList();
		clearAlbumFields();
		size_t len = list.getTrackCount();
		vector<wstring> item(6);
		for(size_t i = 1; i <= len; ++ i)
		{
			for(size_t j = 0; j < (sizeof(fields) / sizeof(*fields)); ++ j)
				item[j] = list[i][fields[j]];
			trackList.addItem(item);
		}

		setAlbumField(L"Album Title", static_cast<wstring>(list[L"Album Title"]));
		setAlbumField(L"Album Artist", static_cast<wstring>(list[L"Album Artist"]));
		setAlbumField(L"Date", static_cast<wstring>(list[L"Date"]));
		setAlbumField(L"Genre", static_cast<wstring>(list[L"Genre"]));
	}

	void MainWindow::clearTrackList()
	{
		trackList.clear();
	}

	namespace
	{
		class Fields : public Singleton<Fields>
		{
		private:
			map<wstring, int32_t> fieldMap;

		private:
			Fields();

		public:
			int32_t getFieldID(const wstring &) const;
			const map<wstring, int32_t> &getMap() const;

			friend class Singleton<Fields>;
		};

		Fields::Fields()
		{
			fieldMap[L"Album Title"] = IDC_ALBUM_TITLE;
			fieldMap[L"Album Artist"] = IDC_ALBUM_ARTIST;
			fieldMap[L"Date"] = IDC_DATE;
			fieldMap[L"Disc Number"] = IDC_DISC_CURRENT;
			fieldMap[L"Total Discs"] = IDC_DISC_TOTAL;
			fieldMap[L"Genre"] = IDC_GENRE;
		}

		int32_t Fields::getFieldID(const wstring &field) const
		{
			auto it = fieldMap.find(field);
			if(it == fieldMap.end())
				return 0;
			return it->second;
		}

		const map<wstring, int32_t> &Fields::getMap() const
		{
			return fieldMap;
		}
	}

	void MainWindow::setAlbumField(const wstring &field, const wstring &value)
	{
		int32_t id = Fields::instance().getFieldID(field);
		if(id != 0)
			SendMessageW(getItemWindow(id), WM_SETTEXT, 0, reinterpret_cast<LPARAM>(value.c_str()));
	}

	void MainWindow::clearAlbumFields()
	{
		const map<wstring, int32_t> &map = Fields::instance().getMap();
		for_each(map.begin(), map.end(), [this](const pair<wstring, int32_t> &item)
		{
			SendMessageW(getItemWindow(item.second), WM_SETTEXT, 0, reinterpret_cast<LPARAM>(L""));
		});

		SendMessageW(getItemWindow(IDC_SAME_ARTIST), BM_SETCHECK, BST_UNCHECKED, 0);
	}
}
