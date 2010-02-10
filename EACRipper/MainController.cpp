#include "Defaults.h"

#include "MainController.h"
#include "MainWindow.h"

namespace EACRipper
{
	MainController::MainController()
	{
		mainWin = &MainWindow::instance();
		mainWin->setOwner(this);
	}

	MainController::~MainController()
	{
	}

	HINSTANCE MainController::getInstance()
	{
		return inst;
	}

	bool MainController::run(HINSTANCE instHandle)
	{
		inst = instHandle;
		return mainWin->show();
	}
}

int __stdcall wWinMain(HINSTANCE inst, HINSTANCE, LPWSTR, int)
{
	return ER::MainController::instance().run(inst) ? 0 : 1;
}
