#pragma once

#include "Singleton.h"
#include "MainWindow.h"
#include "WindowEventArgs.h"

namespace EACRipper
{
	class MainController : public Singleton<MainController>
	{
	protected:
		HINSTANCE inst;
		MainWindow *mainWin;

	protected:
		MainController();
		virtual ~MainController();

	public:
		HINSTANCE getInstance();

	public:
		bool run(HINSTANCE);

	public:
		;

		friend class Singleton<MainController>;
	};
}
