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
		intptr_t procMessageImpl(HWND, uint32_t, uintptr_t, longptr_t);

		static intptr_t __stdcall procMessage(HWND, uint32_t, uintptr_t, longptr_t);
		static int __stdcall procBrowsePath(HWND, uint32_t, longptr_t, longptr_t);

	public:
		virtual const wchar_t *getDialogName();

		virtual bool show();

		std::wstring getValue(const std::wstring &);
		bool setValue(const std::wstring &, const std::wstring &);

		friend class Singleton<PreferenceWindow>;
	};
}
