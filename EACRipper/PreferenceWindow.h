#pragma once

#include "Window.h"
#include "Singleton.h"
#include "Configure.h"

namespace EACRipper
{
	class PreferenceWindow : public Window<MainController>
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
		virtual bool show();
	};
}
