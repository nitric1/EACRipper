#include "Defaults.h"

#include "Configure.h"
#include "FileStream.h"
#include "ComponentServiceImpl.h"
#include "MainController.h"
#include "MainWindow.h"
#include "PreferenceWindow.h"
#include "AboutWindow.h"
#include "CharsetFileDialog.h"
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
		mainWin->addEventListener(L"about", delegateEvent(this, &MainController::onAbout));

		PreferenceWindow *prefWin = &PreferenceWindow::instance();

		prefWin->addEventListener(L"prefInit", delegateEvent(this, &MainController::onPrefInit));
		prefWin->addEventListener(L"prefOK", delegateEvent(this, &MainController::onPrefOK));
	}

	wstring MainController::findLinkedMusicFile(const wstring &ifile, bool last) const
	{
		MusicCoderManager &mcm = MusicCoderManager::instance();
		wstring file = ifile;
		size_t dotp = file.find_last_of(L".");
		wstring path = getDirectoryPath(static_cast<wstring>((*list)[L"CuesheetPath"]));
		wstring base = path + file.substr(0, dotp), ext;
		if(dotp != wstring::npos)
			ext = file.substr(++ dotp);
		LocalFile lf;
		lf.open(file.c_str());
		if(!lf.exists() || mcm.getCoderByExtension(ext, MusicCoderManager::Decoder).empty())
		{
			vector<pair<wstring, int_fast32_t>> exts = mcm.extensions();
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
					throw(runtime_error("Cannot find a linked music file."));
				else
					return findLinkedMusicFile(getFileName(static_cast<wstring>((*list)[L"CuesheetPath"])), true);
			}
		}

		return file;
	}

	void MainController::setTrackDetail()
	{
		wstring file = findLinkedMusicFile((*list)[L"File"]);
		wstring ext = file.substr(file.find_last_of(L".") + 1);
		LocalFile lf;
		lf.open(file.c_str());

		MusicCoderManager &mcm = MusicCoderManager::instance();
		size_t tracks = list->getTrackCount();
		IERAllocator *alloc = MusicCoderManager::instance().getCoder(make_pair(mcm.getCoderByExtension(ext, MusicCoderManager::Decoder), MusicCoderManager::Decoder));
		if(alloc == nullptr)
		{
			throw(runtime_error("Cannot get a coder of a linked music file."));
		}
		IERComponentMusicDecoder *dec = static_cast<IERComponentMusicDecoder *>(alloc->alloc());
		IERStreamReader *r = lf.getStreamReader();
		if(r == nullptr || !dec->setStream(r))
		{
			throw(runtime_error("Cannot open a linked music file."));
		}

		wstring eof = makeTimeString(static_cast<int32_t>(dec->getLength()));

		dec->close();
		alloc->free(dec);
		ServicePointerManager::instance().remove(r);

		wstring tmp, start, end;

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

	bool MainController::run(HINSTANCE instHandle, const wstring &commandLine, int showCommand)
	{
		__try
		{
			return runImpl(instHandle, commandLine, showCommand);
		}
		__except(filterOSException(GetExceptionCode(), GetExceptionInformation()))
		{
			return false;
		}
	}

	bool MainController::runImpl(HINSTANCE instHandle, const wstring &commandLine, int showCommand)
	{
		try
		{
			inst = instHandle;

			registerEvents();

			mainWin->setShowStatus(showCommand);
			return mainWin->show();
		}
		catch(exception &e)
		{
			string str = string("Unexpected exception occured: ") + e.what() + string("\nThe program will be terminated.");
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
		return false;
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

		CharsetFileDialog fd(true, mainWin, L"Open Cuesheet", fi, L"cue");
		if(fd.show())
		{
			FileStreamReader f;
			if(!f.open(fd.getPath().c_str()))
				return false;
			vector<char> ve(static_cast<size_t>(f.size()) + 1);
			f.read(&*ve.begin(), ve.size());
			ve.back() = '\0';
			f.close();

			// TODO: Charset detection or user-selected encoding
			CharsetDetector &cd = CharsetDetector::instance();
			IERServiceStringConverter *cv = cd.detect(&*ve.begin());
			vector<wchar_t> doc(cv->getConvertedLengthToUTF16(&*ve.begin()) + 1);
			cv->convertToUTF16(&*doc.begin(), doc.size(), &*ve.begin());
			doc.back() = L'\0';
			ServicePointerManager::instance().remove(cv);

			CuesheetTrackList *plist = new CuesheetTrackList(wstring(&*doc.begin()));
			list = shared_ptr<TrackList>(plist);

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

		/*const vector<wstring> &list = StringCharsetConverter::getCharsetList();
		for_each(list.begin(), list.end(), [&prefWin](const wstring &name) { prefWin->addCharsetName(name); });
		prefWin->setValue(L"DetectPriority", c.get(L"DetectPriority", L"UTF-8"));*/

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
		//c.set(L"DetectPriority", prefWin->getValue(L"DetectPriority"));
		return true;
	}
}

int __stdcall wWinMain(HINSTANCE inst, HINSTANCE, wchar_t *commandLine, int showCommand)
{
	return ER::MainController::instance().run(inst, commandLine, showCommand) ? 0 : 1;
}
