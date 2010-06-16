#pragma once

#include "ChildWindow.h"
#include "Singleton.h"

namespace EACRipper
{
	class AboutWindow : public ChildWindow, public Singleton<AboutWindow>
	{
	private:
		enum { DIALOG_ID = IDD_ABOUT };

	private:
		bool mouseOver;

	private:
		AboutWindow();
		~AboutWindow();

	private:
		WNDPROC linkOldProc;

		static intptr_t __stdcall procMessage(HWND, unsigned, WPARAM, LPARAM);
		static uintptr_t __stdcall procLink(HWND, unsigned, WPARAM, LPARAM);

	public:
		virtual bool show();

		friend class Singleton<AboutWindow>;
	};
}
