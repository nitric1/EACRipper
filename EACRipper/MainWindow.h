#pragma once

#include "Dialog.h"
#include "Singleton.h"
#include "ShortcutKey.h"
#include "ComponentManager.h"
#include "EditListControl.h"
#include "TrackList.h"

namespace EACRipper
{
	class MainWindow : public Dialog, public Singleton<MainWindow>
	{
	private:
		typedef ComponentManager CM;

		enum { DIALOG_ID = IDD_MAIN_DIALOG };

	private:
		HICON iconSmall, iconBig;
		ShortcutKey *shortcut;
		int baseUnitX, baseUnitY;
		Gdiplus::Image *coverArtThumbnail;
		int coverArtLeft, coverArtTop, coverArtWidth, coverArtHeight;
		EditListControl trackList;

	private:
		MainWindow();
		~MainWindow();

	private:
		WNDPROC coverArtOldProc;

		static intptr_t __stdcall procMessage(HWND, unsigned, WPARAM, LPARAM);
		static uintptr_t __stdcall procCoverArt(HWND, unsigned, WPARAM, LPARAM);

	public:
		virtual const wchar_t *getDialogName();

		virtual bool show();
		void resizeLayout(int, int);
		void resizeItemDLU(int, int, int, int, int);

		void initList();
		void uninit();

		void addFormat(const std::wstring &);
		void selectFormat(const std::wstring &);

		void setCoverArt(const std::wstring &);
		void cancelCoverArt();

		void setTrackList(const TrackList &);
		void clearTrackList();

		friend class Singleton<MainWindow>;
	};
}
