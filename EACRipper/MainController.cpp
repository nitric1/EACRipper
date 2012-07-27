#include "Defaults.h"

#include "MainController.h"

#include "AboutWindow.h"
#include "CharsetFileDialog.h"
#include "ComponentServiceImpl.h"
#include "Configure.h"
#include "Delegate.h"
#include "FileStream.h"
#include "MainWindow.h"
#include "PreferenceWindow.h"
#include "RipManager.h"
#include "TrackList.h"
#include "Utility.h"

namespace EACRipper
{
	using namespace ServiceImpl;

	MainController::MainController()
	{
		mainWin = &MainWindow::instance();
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
		Gdiplus::GdiplusStartupInput gsi;

		Gdiplus::GdiplusStartup(&gpToken, &gsi, nullptr);

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

		Gdiplus::GdiplusShutdown(gpToken);
	}

	void MainController::registerEvents()
	{
		mainWin->addEventListener(L"init", delegateWindowEvent(this, &MainController::onInit));
		mainWin->addEventListener(L"close", delegateWindowEvent(this, &MainController::onClose));
		mainWin->addEventListener(L"openCuesheet", delegateWindowEvent(this, &MainController::onOpenCuesheet));
		mainWin->addEventListener(L"openInCue", delegateWindowEvent(this, &MainController::onOpenInCue));
		mainWin->addEventListener(L"openArchive", delegateWindowEvent(this, &MainController::onOpenArchive));
		mainWin->addEventListener(L"option", delegateWindowEvent(this, &MainController::onOption));
		mainWin->addEventListener(L"setCoverArt", delegateWindowEvent(this, &MainController::onSetCoverArt));
		mainWin->addEventListener(L"cancelCoverArt", delegateWindowEvent(this, &MainController::onCancelCoverArt));
		mainWin->addEventListener(L"rip", delegateWindowEvent(this, &MainController::onRip));
		mainWin->addEventListener(L"about", delegateWindowEvent(this, &MainController::onAbout));

		PreferenceWindow *prefWin = &PreferenceWindow::instance();

		prefWin->addEventListener(L"prefInit", delegateWindowEvent(this, &MainController::onPrefInit));
		prefWin->addEventListener(L"prefOK", delegateWindowEvent(this, &MainController::onPrefOK));
	}

	std::wstring MainController::findLinkedMusicFile(const std::wstring &ifile, bool last) const
	{
		MusicCoderManager &mcm = MusicCoderManager::instance();
		std::wstring file = ifile;
		size_t dotp = file.find_last_of(L".");
		std::wstring path = getDirectoryPath(static_cast<std::wstring>((*list)[L"CuesheetPath"]));
		std::wstring base = path + file.substr(0, dotp), ext;
		if(dotp != std::wstring::npos)
			ext = file.substr(++ dotp);
		LocalFile lf;
		lf.open(file.c_str());
		if(!lf.exists() || mcm.getCoderByExtension(ext, MusicCoderManager::Decoder).empty())
		{
			std::vector<std::pair<std::wstring, int_fast32_t>> exts = mcm.extensions();
			auto it = exts.begin();
			for(; it != exts.end(); ++ it)
			{
				if(it->second != MusicCoderManager::Decoder)
					continue;
				file = base;
				file += L".";
				file += it->first;
				lf.open(file.c_str());
				if(lf.exists())
				{
					ext = it->first;
					break;
				}
			}

			if(it == exts.end())
			{
				if(last)
					throw(std::runtime_error("Cannot find a linked music file."));
				else
					return findLinkedMusicFile(getFileName(static_cast<std::wstring>((*list)[L"CuesheetPath"])), true);
			}
		}

		return file;
	}

	void MainController::setTrackDetail()
	{
		std::wstring file = findLinkedMusicFile((*list)[L"File"]);
		(*list)[L"SourcePath"] = file;
		std::wstring ext = file.substr(file.find_last_of(L".") + 1);
		LocalFile lf;
		lf.open(file.c_str());

		MusicCoderManager &mcm = MusicCoderManager::instance();
		size_t tracks = list->getTrackCount();
		IERAllocator *alloc = mcm.getCoder(make_pair(mcm.getCoderByExtension(ext, MusicCoderManager::Decoder), MusicCoderManager::Decoder));
		if(alloc == nullptr)
		{
			throw(std::runtime_error("Cannot get a coder of a linked music file."));
		}
		IERComponentMusicDecoder *dec = static_cast<IERComponentMusicDecoder *>(alloc->alloc());
		IERStreamReader *r = lf.getStreamReader();
		if(r == nullptr || !dec->setStream(r))
		{
			throw(std::runtime_error("Cannot open a linked music file."));
		}

		std::wstring eof = makeTimeString(static_cast<int32_t>(dec->getLength()));

		dec->close();
		alloc->free(dec);
		ServicePointerManager::instance().remove(r);

		std::wstring tmp, start, end;

		for(size_t i = 1; i <= tracks; ++ i)
		{
			start = (*list)[i][L"Start Time"];
			end = (*list)[i][L"End Time"];
			if(end == L"EOF")
				end = eof;
			tmp = getTimeStringDiff(start, end);
			tmp += L"(";
			tmp += start;
			tmp += L"-";
			tmp += end;
			tmp += L")";

			(*list)[i][L"Length"] = tmp;
		}
	}

	bool MainController::run(HINSTANCE instHandle, const std::wstring &commandLine, int showCommand)
	{
		//__try
		{
			return runImpl(instHandle, commandLine, showCommand);
		}
		//__except(filterOSException(GetExceptionCode(), GetExceptionInformation()))
		{
		//	return false;
		}
	}

	bool MainController::runImpl(HINSTANCE instHandle, const std::wstring &commandLine, int showCommand)
	{
		try
		{
#ifdef _WIN64
			HMODULE inoutWave = LoadLibraryW(L"J:\\App\\VC++\\EACRipper\\x64\\Debug\\Components\\InoutWave.dll");
			void (*occur)() = reinterpret_cast<void (*)()>(GetProcAddress(inoutWave, "occur"));
			occur();
			FreeLibrary(inoutWave);
#endif

			inst = instHandle;

			registerEvents();

			mainWin->setShowStatus(showCommand);
			return mainWin->show();
		}
		catch(std::exception &e)
		{
			std::string str = "Unexpected exception occured: ";
			str += e.what();
			str += "\nThe program will be terminated.";
			MessageBoxA(nullptr, str.c_str(), "Error occured", MB_ICONSTOP | MB_OK);
			return false;
		}
	}

	int MainController::filterOSException(unsigned icode, EXCEPTION_POINTERS *iep)
	{
		exceptionCode = icode;
		memcpy(&exceptionPointers, iep, sizeof(exceptionPointers));

		return showOSException() ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH;
	}

	bool MainController::showOSException()
	{
		// TODO: Implementaion exception showing
		return true;
	}

	void MainController::invoke(std::shared_ptr<ERDelegate<void ()>> dg)
	{
		mainWin->invoke(dg);
	}

	bool MainController::onInit(WindowEventArgs e)
	{
		initializeApp();

		Configure &conf = Configure::instance();
		int64_t x, y, width, height;
		x = conf.getInt(L"WindowPosX", std::numeric_limits<int64_t>::min());
		y = conf.getInt(L"WindowPosY", std::numeric_limits<int64_t>::min());
		width = conf.getInt(L"WindowWidth", std::numeric_limits<int64_t>::min());
		height = conf.getInt(L"WindowHeight", std::numeric_limits<int64_t>::min());

		if(!(x == std::numeric_limits<int64_t>::min() || y == std::numeric_limits<int64_t>::min() || width == std::numeric_limits<int64_t>::min() || height == std::numeric_limits<int64_t>::min()))
		{
			MoveWindow(mainWin->getWindow(), static_cast<int32_t>(x), static_cast<int32_t>(y), static_cast<int32_t>(width), static_cast<int32_t>(height), TRUE);
		}

		return true;
	}

	bool MainController::onClose(WindowEventArgs e)
	{
		WINDOWPLACEMENT wp;

		wp.length = sizeof(wp);
		GetWindowPlacement(mainWin->getWindow(), &wp);

		Configure &conf = Configure::instance();
		conf.setInt(L"WindowPosX", wp.rcNormalPosition.left);
		conf.setInt(L"WindowPosY", wp.rcNormalPosition.top);
		conf.setInt(L"WindowWidth", wp.rcNormalPosition.right - wp.rcNormalPosition.left);
		conf.setInt(L"WindowHeight", wp.rcNormalPosition.bottom - wp.rcNormalPosition.top);

		uninitializeApp();
		return true;
	}

	bool MainController::onOpenCuesheet(WindowEventArgs e)
	{
		FileDialogFilter fi;
		fi.add(L"Cuesheet", L"*.cue");

		CharsetFileDialog fd(true, mainWin, L"Open Cuesheet", fi, L"cue");
		if(fd.show())
		{
			FileStreamReader f;
			if(!f.open(fd.getPath().c_str()))
				return false;
			std::vector<char> ve(static_cast<size_t>(f.size()) + 2);
			f.read(&*ve.begin(), ve.size());
			f.close();

			std::string cs = fd.getCharset();
			std::vector<wchar_t> doc;
			if(cs == "Auto")
			{
				CharsetDetector &cd = CharsetDetector::instance();
				IERServiceStringConverter *cv = cd.detect(&*ve.begin());
				size_t len = cv->getConvertedLengthToUTF16(&*ve.begin());
				if(len == std::numeric_limits<size_t>::max())
				{
					// TODO: Cannot convert the string
				}
				doc.resize(len + 1);
				cv->convertToUTF16(&*doc.begin(), doc.size(), &*ve.begin());
				ServicePointerManager::instance().remove(cv);
			}
			else
			{
				StringCharsetConverter cv;
				cv.setCharset(cs.c_str());
				size_t len = cv.getConvertedLengthToUTF16(&*ve.begin());
				if(len == std::numeric_limits<size_t>::max())
				{
					// TODO: Cannot convert the string
				}
				doc.resize(len + 1);
				cv.convertToUTF16(&*doc.begin(), doc.size(), &*ve.begin());
			}

			CuesheetTrackList *plist = new CuesheetTrackList(std::wstring(&*doc.begin()));
			list = std::shared_ptr<TrackList>(plist);

			(*list)[L"CuesheetPath"] = fd.getPath();

			setTrackDetail();

			mainWin->setTrackList(*plist);
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
		std::wstring ext;
		std::vector<std::wstring> ve, allExt;
		std::vector<std::pair<std::wstring, std::wstring>> fiVec;
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

				std::for_each(ve.begin(), ve.end(), [](std::wstring &str) { str = L"*." + str; });
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

		CharsetFileDialog fd(true, mainWin, L"Open InCue File", fi);
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
		CharsetFileDialog fd(true, mainWin, L"Open Archive File", fi);
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
		//RipManager::instance().startRip(*list, delegateRipCallback(this, &ripCallback));
		return true;
	}

	bool MainController::onAbout(WindowEventArgs e)
	{
		AboutWindow &win = AboutWindow::instance();
		win.showWithParent(mainWin);
		return true;
	}

	bool MainController::onPrefInit(WindowEventArgs e)
	{
		Configure &c = Configure::instance();
		PreferenceWindow *prefWin = dynamic_cast<PreferenceWindow *>(e.window);
		if(prefWin == nullptr)
			return false;

		prefWin->setValue(L"BasePath", c.get(L"BasePath"));
		prefWin->setValue(L"NameFormat", c.get(L"NameFormat"));
		prefWin->setValue(L"RipGap", c.get(L"RipGap", L"No"));
		prefWin->setValue(L"AttachGapNextTrack", c.get(L"AttachGapNextTrack", L"No"));
		prefWin->setValue(L"IgnorePregapPostgap", c.get(L"IgnorePregapPostgap", L"No"));

		return true;
	}

	bool MainController::onPrefOK(WindowEventArgs e)
	{
		Configure &c = Configure::instance();
		PreferenceWindow *prefWin = dynamic_cast<PreferenceWindow *>(e.window);
		if(prefWin == nullptr)
			return false;

		c.set(L"BasePath", prefWin->getValue(L"BasePath"));
		c.set(L"NameFormat", prefWin->getValue(L"NameFormat"));
		c.set(L"RipGap", prefWin->getValue(L"RipGap"));
		c.set(L"AttachGapNextTrack", prefWin->getValue(L"AttachGapNextTrack"));
		c.set(L"IgnorePregapPostgap", prefWin->getValue(L"IgnorePregapPostgap"));

		return true;
	}

	bool MainController::ripCallback(size_t track, uint64_t, uint64_t, int32_t)
	{
		return true;
	}
}

int __stdcall wWinMain(HINSTANCE inst, HINSTANCE, wchar_t *commandLine, int showCommand)
{
	return ER::MainController::instance().run(inst, commandLine, showCommand) ? 0 : 1;
}
