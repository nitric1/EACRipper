#include "Defaults.h"

#include "MainController.h"
#include "MainWindow.h"

using namespace Gdiplus;

#include "../Common/ERUUID.h"

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
		char uuid[50];
		wchar_t uuidw[50];

		ERUUID uuid1, uuid2("550e8400-e29b-41d4-a716-446655440000");

		uuid1 = ERUUID(0x037fc123u, 0x501d, 0x3033, 0xa8ca, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06);
		ERUUID uuid3 = uuid1;
		uuid1 = uuid2;

		uuid3.toString(uuid);
		uuid1.toString(uuidw);

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

	bool MainController::onOpenCuesheet(WindowEventArgs e)
	{
		return true;
	}

	bool MainController::onOpenArchive(WindowEventArgs e)
	{
		return true;
	}

	bool MainController::onOption(WindowEventArgs e)
	{
		return true;
	}

	bool MainController::onRip(WindowEventArgs e)
	{
		return true;
	}
}

int __stdcall wWinMain(HINSTANCE inst, HINSTANCE, LPWSTR, int)
{
	return ER::MainController::instance().run(inst) ? 0 : 1;
}
