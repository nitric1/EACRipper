#pragma once

#include "Window.h"
#include "Singleton.h"
#include "ShortcutKey.h"
#include "ComponentManager.h"

namespace EACRipper
{
	class MainController;

	class MainWindow : public Window<MainController>, public Singleton<MainWindow>
	{
	private:
		typedef ComponentManager CM;

		enum { DIALOG_ID = IDD_MAIN_DIALOG };

	private:
		HICON iconSmall, iconBig;
		ShortcutKey *shortcut;

	private:
		MainWindow();
		~MainWindow();

	private:
		static intptr_t __stdcall procMessage(HWND, unsigned, WPARAM, LPARAM);

	public:
		bool show();

		friend class Singleton<MainWindow>;
	};
}
