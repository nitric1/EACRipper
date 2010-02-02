#pragma once

#include "Singleton.h"

namespace EACRipper
{
	class MainWindow : public Singleton<MainWindow>
	{
	protected:
		enum { DIALOG_ID = IDD_MAIN_DIALOG };

	protected:
		HWND window;

	protected:
		MainWindow();
		~MainWindow();

		friend class Singleton<MainWindow>;
	};
}
