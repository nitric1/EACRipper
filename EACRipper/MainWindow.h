#pragma once

#include "Window.h"
#include "Singleton.h"
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
		HWND window;

		HICON iconSmall, iconBig;

	private:
		MainWindow();
		~MainWindow();

	public:
		HWND getWindow();

	private:
		static BOOL __stdcall procMessage(HWND, unsigned, WPARAM, LPARAM);

	public:
		bool show();

		friend class Singleton<MainWindow>;
	};
}
