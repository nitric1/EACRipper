#include "Defaults.h"

#include "Configure.h"
#include "FileStream.h"
#include "ComponentServiceImpl.h"
#include "MainController.h"
#include "MainWindow.h"
#include "PreferenceWindow.h"
#include "FileDialog.h"
#include "TrackList.h"
#include "Utility.h"

using namespace Gdiplus;
using namespace std;
using namespace std::tr1;

namespace EACRipper
{
	using namespace ServiceImpl;

	MainController::MainController()
	{
		mainWin = &MainWindow::instance();
		/*mainWin->setOwner(this);*/
	}

	MainController::~MainController()
	{
	}

	HINSTANCE MainController::getInstance() const
	{
		return inst;
	}

	void MainController::initializeApp()
	{
		GdiplusStartupInput gsi;

		GdiplusStartup(&gpToken, &gsi, nullptr);

		OleInitialize(nullptr);

		comp = &ComponentManager::instance();

		MusicCoderManager *cd = &MusicCoderManager::instance();
		auto v = cd->coders();
		for(auto it = v.begin(); it != v.end(); ++ it)
		{
			if(it->second == MusicCoderManager::Encoder)
				mainWin->addFormat(it->first);
		}

		if(!v.empty())
			mainWin->selectFormat(v[0].first);
	}

	void MainController::uninitializeApp()
	{
		OleUninitialize();

		GdiplusShutdown(gpToken);
	}

	void MainController::registerEvents()
	{
		mainWin->addEventListener(L"init", delegateEvent(this, &MainController::onInit));
		mainWin->addEventListener(L"close", delegateEvent(this, &MainController::onClose));
		mainWin->addEventListener(L"openCuesheet", delegateEvent(this, &MainController::onOpenCuesheet));
		mainWin->addEventListener(L"openInCue", delegateEvent(this, &MainController::onOpenInCue));
		mainWin->addEventListener(L"openArchive", delegateEvent(this, &MainController::onOpenArchive));
		mainWin->addEventListener(L"option", delegateEvent(this, &MainController::onOption));
		mainWin->addEventListener(L"setCoverArt", delegateEvent(this, &MainController::onSetCoverArt));
		mainWin->addEventListener(L"cancelCoverArt", delegateEvent(this, &MainController::onCancelCoverArt));
		mainWin->addEventListener(L"rip", delegateEvent(this, &MainController::onRip));

		mainWin->addEventListener(L"prefInit", delegateEvent(this, &MainController::onPrefInit));
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
		FileDialogFilter fi;
		fi.add(L"Cuesheet", L"*.cue");
		FileDialog fd(true, mainWin, L"Open Cuesheet", fi, L"cue");
		if(fd.show())
		{
			// TODO: Open Cuesheet Implementation
			FileStreamReader f;
			if(!f.open(fd.getPath().c_str()))
				return false;
			vector<char> ve(static_cast<size_t>(f.size()) + 1);
			f.read(&*ve.begin(), ve.size());
			ve.back() = '\0';
			f.close();

			// TODO: Charset detection or user-selected encoding
			CharsetDetector cd;
			IERServiceStringConverter *cv = cd.detect(&*ve.begin());
			vector<wchar_t> doc(cv->getConvertedLengthToUTF16(&*ve.begin()) + 1);
			cv->convertToUTF16(&*doc.begin(), doc.size(), &*ve.begin());
			doc.back() = L'\0';

			CuesheetTrackList *ptr = new CuesheetTrackList(wstring(&*doc.begin()));
			shared_ptr<TrackList> tr(ptr);

			// mainWin->setTrackList(tr);
		}

		return true;
	}

	bool MainController::onOpenInCue(WindowEventArgs e)
	{
		FileDialogFilter fi;

		MusicCoderManager *mcm;
		IERAllocator *alloc;

		mcm = &MusicCoderManager::instance();
		auto cd = mcm->coders();
		wstring ext;
		vector<wstring> ve, allExt;
		vector<pair<wstring, wstring>> fiVec;
		IERComponentInCueMusicDecoder *dec;
		for(auto it = cd.begin(); it != cd.end(); ++ it)
		{
			if(it->second == MusicCoderManager::InCueDecoder)
			{
				alloc = mcm->getCoder(*it);
				dec = static_cast<IERComponentInCueMusicDecoder *>(alloc->alloc());
				const DecoderInformation &info = dec->getInfo();
				ve = move(split(info.extension, L";"));
				alloc->free(dec);

				for_each(ve.begin(), ve.end(), [](wstring &str) { str = L"*." + str; });
				ext = join(ve, L";");
				fiVec.push_back(make_pair(it->first, ext));
				allExt.push_back(ext);
			}
		}

		if(!fiVec.empty())
		{
			fi.add(L"All Supported inCue Files", join(allExt, L";"));
			fi.add(fiVec);
		}

		FileDialog fd(true, mainWin, L"Open InCue File", fi);
		if(fd.show())
		{
			// TODO: Open InCue Implementation
		}

		return true;
	}

	bool MainController::onOpenArchive(WindowEventArgs e)
	{
		FileDialogFilter fi;
		// TODO: Filter
		FileDialog fd(true, mainWin, L"Open Archive File", fi);
		if(fd.show())
		{
			// TODO: Open Archive Implementation
		}

		return true;
	}

	bool MainController::onOption(WindowEventArgs e)
	{
		PreferenceWindow &win = PreferenceWindow::instance();

		win.showWithParent(mainWin);

		return true;
	}

	bool MainController::onSetCoverArt(WindowEventArgs e)
	{
		FileDialogFilter fi;
		fi.add(L"Image File", L"*.jpg;*.jpeg;*.gif;*.png");
		FileDialog fd(true, mainWin, L"Open Cover Art", fi, L"jpg");
		if(fd.show())
		{
			mainWin->setCoverArt(fd.getPath());
		}

		return true;
	}

	bool MainController::onCancelCoverArt(WindowEventArgs e)
	{
		mainWin->cancelCoverArt();
		return true;
	}

	bool MainController::onRip(WindowEventArgs e)
	{
		return true;
	}

	bool MainController::onPrefInit(WindowEventArgs e)
	{
		Configure &c = Configure::instance();
		static_cast<PreferenceWindow *>(e.window)->setValue(L"BasePath", c.get(L"BasePath"));

		return true;
	}
}

int __stdcall wWinMain(HINSTANCE inst, HINSTANCE, wchar_t *, int)
{
	return ER::MainController::instance().run(inst) ? 0 : 1;
}
