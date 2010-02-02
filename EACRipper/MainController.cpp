#include "Defaults.h"

#include "MainController.h"
#include "MainWindow.h"

namespace EACRipper
{
	MainController::MainController()
	{
	}

	MainController::~MainController()
	{
	}

	int MainController::run(HINSTANCE instHandle)
	{
		inst = instHandle;
		return 0;
	}
}

int __stdcall wWinMain(HINSTANCE inst, HINSTANCE, LPWSTR, int)
{
	return EACRipper::MainController::instance().run(inst);
}
