#pragma once

#include "Singleton.h"
#include "MainWindow.h"
#include "WindowEventArgs.h"

namespace EACRipper
{
	class MainController : public Singleton<MainController>
	{
	private:
		HINSTANCE inst;
		MainWindow *mainWin;

		ULONG_PTR gpToken;

	private:
		MainController();
		~MainController();

	public:
		HINSTANCE getInstance();

	public:
		void registerEvents();
		void initializeApp();
		void uninitializeApp();
		bool run(HINSTANCE);

	public:
		bool onInit(WindowEventArgs);
		bool onClose(WindowEventArgs);
		bool onOpenCuesheet(WindowEventArgs);
		bool onOpenArchive(WindowEventArgs);
		bool onOption(WindowEventArgs);
		bool onRip(WindowEventArgs);

		friend class Singleton<MainController>;
	};
}