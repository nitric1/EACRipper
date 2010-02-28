#include "Defaults.h"

#include "MainController.h"
#include "MainWindow.h"

using namespace Gdiplus;

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

	void MainController::initializeApp()
	{
		GdiplusStartupInput gsi;

		GdiplusStartup(&gpToken, &gsi, NULL);

		OleInitialize(NULL);
	}

	void MainController::uninitializeApp()
	{
		OleUninitialize();

		GdiplusShutdown(gpToken);
	}

	void MainController::registerEvents()
	{
		mainWin->addEventListener(L"init", &MainController::onInit);
		mainWin->addEventListener(L"close", &MainController::onClose);
	}

	bool MainController::run(HINSTANCE instHandle)
	{
		inst = instHandle;

		registerEvents();

		return mainWin->show();
	}

	bool MainController::onInit(WindowEventArgs e)
	{
		initializeApp();
		return true;
	}

	bool MainController::onClose(WindowEventArgs e)
	{
		uninitializeApp();
		return true;
	}
}

int __stdcall wWinMain(HINSTANCE inst, HINSTANCE, LPWSTR, int)
{
	return ER::MainController::instance().run(inst) ? 0 : 1;
}
