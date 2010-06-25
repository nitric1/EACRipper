#pragma once

#include "Singleton.h"
#include "MainWindow.h"
#include "ComponentManager.h"
#include "CoderManager.h"

namespace EACRipper
{
	class MainController : public Singleton<MainController>
	{
	private:
		HINSTANCE inst;
		MainWindow *mainWin;
		ComponentManager *comp;

		ULONG_PTR gpToken;

	private:
		MainController();
		~MainController();

	public:
		HINSTANCE getInstance() const;

	public:
		void registerEvents();
		void initializeApp();
		void uninitializeApp();
		bool run(HINSTANCE, const std::wstring &, int);

	public:
		bool onInit(WindowEventArgs);
		bool onClose(WindowEventArgs);
		bool onOpenCuesheet(WindowEventArgs);
		bool onOpenInCue(WindowEventArgs);
		bool onOpenArchive(WindowEventArgs);
		bool onOption(WindowEventArgs);
		bool onSetCoverArt(WindowEventArgs);
		bool onCancelCoverArt(WindowEventArgs);
		bool onRip(WindowEventArgs);
		bool onAbout(WindowEventArgs);

		bool onPrefInit(WindowEventArgs);

		friend class Singleton<MainController>;
	};
}
