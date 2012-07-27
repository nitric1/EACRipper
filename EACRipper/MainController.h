#pragma once

#include "CoderManager.h"
#include "ComponentManager.h"
#include "Delegate.h"
#include "MainWindow.h"
#include "Singleton.h"

namespace EACRipper
{
	class MainController : public Singleton<MainController>
	{
	private:
		HINSTANCE inst;
		MainWindow *mainWin;
		ComponentManager *comp;
		std::shared_ptr<TrackList> list;
		unsigned exceptionCode;
		EXCEPTION_POINTERS exceptionPointers;

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

	private:
		std::wstring findLinkedMusicFile(const std::wstring &, bool = false) const;
		void setTrackDetail();

	public:
		bool run(HINSTANCE, const std::wstring &, int);
		bool runImpl(HINSTANCE, const std::wstring &, int);
		int filterOSException(unsigned, EXCEPTION_POINTERS *);
		bool showOSException();
		void invoke(std::shared_ptr<ERDelegate<void ()>>);

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
		bool onPrefOK(WindowEventArgs);

		bool ripCallback(size_t, uint64_t, uint64_t, int32_t);

		friend class Singleton<MainController>;
	};
}
