#pragma once

#include "Dialog.h"
#include "Singleton.h"
#include "ShortcutKey.h"
#include "ComponentManager.h"
#include "TrackList.h"
#include "TrackListControl.h"

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

		intptr_t procMessageImpl(HWND, unsigned, uintptr_t, longptr_t);
		uintptr_t procCoverArtImpl(HWND, unsigned, uintptr_t, longptr_t);

		static intptr_t __stdcall procMessage(HWND, unsigned, uintptr_t, longptr_t);
		static uintptr_t __stdcall procCoverArt(HWND, unsigned, uintptr_t, longptr_t);

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

		void setAlbumField(const std::wstring &, const std::wstring &);
		void clearAlbumFields();

		friend class Singleton<MainWindow>;
	};
}
