#pragma once

#include "ChildWindow.h"
#include "Singleton.h"
#include "Configure.h"

namespace EACRipper
{
	class PreferenceWindow : public ChildWindow, public Singleton<PreferenceWindow>
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

	public:
		std::wstring getValue(const std::wstring &);
		bool setValue(const std::wstring &, const std::wstring &);
		virtual bool show();

		friend class Singleton<PreferenceWindow>;
	};
}
