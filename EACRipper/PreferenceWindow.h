#pragma once

#include "Dialog.h"
#include "Singleton.h"
#include "Configure.h"

namespace EACRipper
{
	class PreferenceWindow : public Dialog, public Singleton<PreferenceWindow>
	{
	private:
		enum { DIALOG_ID = IDD_OPTION };

	private:
		Configure *conf;

	private:
		PreferenceWindow();
		~PreferenceWindow();

	private:
		static intptr_t __stdcall procMessage(HWND, unsigned, WPARAM, LPARAM);
		static int __stdcall procBrowsePath(HWND, unsigned, LPARAM, LPARAM);

	public:
		virtual const wchar_t *getDialogName();

		virtual bool show();

		std::wstring getValue(const std::wstring &);
		bool setValue(const std::wstring &, const std::wstring &);

		friend class Singleton<PreferenceWindow>;
	};
}
