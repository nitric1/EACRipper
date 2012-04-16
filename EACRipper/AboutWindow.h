#pragma once

#include "Dialog.h"
#include "Singleton.h"

namespace EACRipper
{
	class AboutWindow : public Dialog, public Singleton<AboutWindow>
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

		intptr_t procMessageImpl(HWND, uint32_t, uintptr_t, longptr_t);
		uintptr_t procLinkImpl(HWND, uint32_t, uintptr_t, longptr_t);

		static intptr_t __stdcall procMessage(HWND, uint32_t, uintptr_t, longptr_t);
		static uintptr_t __stdcall procLink(HWND, uint32_t, uintptr_t, longptr_t);

	public:
		virtual const wchar_t *getDialogName();

		virtual bool show();

		friend class Singleton<AboutWindow>;
	};
}
