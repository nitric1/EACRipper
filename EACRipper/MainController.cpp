#include "Defaults.h"

#include "Configure.h"
#include "MainController.h"
#include "MainWindow.h"
#include "FileDialog.h"

using namespace Gdiplus;
using namespace std;

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

		comp = &ComponentManager::instance();

		MusicCoderManager *cd = &MusicCoderManager::instance();
		auto v = cd->coders();
		for(auto it = v.begin(); it != v.end(); ++ it)
		{
			if(it->second == MusicCoderManager::Encoder)
				mainWin->addFormat(it->first);
		}

		mainWin->selectFormat(v[0].first);
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
		mainWin->addEventListener(L"openCuesheet", &MainController::onOpenCuesheet);
		mainWin->addEventListener(L"openArchive", &MainController::onOpenArchive);
		mainWin->addEventListener(L"option", &MainController::onOption);
		mainWin->addEventListener(L"rip", &MainController::onRip);
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

	bool MainController::onOpenCuesheet(WindowEventArgs e)
	{
		FileDialog fd(true, mainWin, L"Open Cuesheet", L"~~", L"cue");
		if(fd.show())
		{
			// some process
		}

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

int __stdcall wWinMain(HINSTANCE inst, HINSTANCE, wchar_t *, int)
{
	return ER::MainController::instance().run(inst) ? 0 : 1;
}
