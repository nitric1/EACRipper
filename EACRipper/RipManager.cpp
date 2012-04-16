#include "Defaults.h"

#include "RipManager.h"

#include "FileStream.h"
#include "Utility.h"
#include "Configure.h"

using namespace std;

namespace EACRipper
{
	RipManager::RipManager()
		: processorCount(1), runningThreads(0), mutex(nullptr), stop(false)
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);

		processorCount = si.dwNumberOfProcessors;
	}

	RipManager::~RipManager()
	{
		stopRip();
	}

	bool RipManager::startRip(const TrackList &ilist, shared_ptr<RipCallbackDelegate> progressCallback)
	{
		if(runningThreads > 0)
			return false;

		tracks.clear();
		threads.clear();
		threadData.clear();

		threads.resize(processorCount);
		threadData.resize(processorCount);

		stop = false;

		list = shared_ptr<TrackList>(ilist.clone());
		size_t len = ilist.getTrackCount();

		for(size_t i = 1; i <= len; ++ i)
			tracks.push_back(i);

		// TODO: Read whole file?
		wstring file = static_cast<wstring>((*list)[L"SourcePath"]);
		wstring ext = file.substr(file.find_last_of(L".") + 1);
		MusicCoderManager &mcm = MusicCoderManager::instance();
		IERAllocator *alloc = mcm.getCoder(make_pair(mcm.getCoderByExtension(ext, MusicCoderManager::Decoder), MusicCoderManager::Decoder));
		if(alloc == nullptr)
		{
			return false;
		}

		mutex = CreateMutexW(NULL, FALSE, NULL);

		for(uint32_t i = 0; i < processorCount; ++ i)
		{
			InterlockedIncrement(&runningThreads);
			threadData[i].threadId = i;
			threadData[i].callback = progressCallback;
			threadData[i].decAlloc = alloc;
			threads[i] = CreateThread(NULL, 0, ripThread, &threadData[i], 0, nullptr);
			if(threads[i] == nullptr)
				InterlockedDecrement(&runningThreads);
		}

		if(runningThreads == 0)
			return false;

		return true;
	}

	bool RipManager::stopRip()
	{
		stop = true;
		if(WaitForMultipleObjects(processorCount, &*threads.begin(), TRUE, INFINITE) == WAIT_FAILED)
			return false;
		if(mutex != nullptr)
		{
			CloseHandle(mutex);
			mutex = nullptr;
		}
		threads.clear();
		threadData.clear();
		return true;
	}

	bool RipManager::isRipping()
	{
		return runningThreads > 0;
	}

	namespace
	{
		class MutexSession
		{
		private:
			HANDLE mutex;
			bool wait;

		public:
			MutexSession(HANDLE, bool = false);
			~MutexSession();

			void waitSession();
			void releaseSession();
		};

		MutexSession::MutexSession(HANDLE imutex, bool iwait)
			: mutex(imutex), wait(false)
		{
			if(iwait)
				waitSession();
		}

		MutexSession::~MutexSession()
		{
			releaseSession();
		}

		void MutexSession::waitSession()
		{
			if(wait)
				return;

			if(WaitForSingleObject(mutex, INFINITE) != WAIT_FAILED)
				wait = true;
		}

		void MutexSession::releaseSession()
		{
			if(!wait)
				return;

			if(ReleaseMutex(mutex))
				wait = false;
		}
	}

	void RipManager::ripThreadImpl(ThreadData *data)
	{
		size_t track;
		uint64_t samples;

		Configure &conf = Configure::instance();

		FileStreamReader fsr(static_cast<wstring>((*list)[L"SourcePath"]).c_str());
		IERComponentMusicDecoder *dec = static_cast<IERComponentMusicDecoder *>(data->decAlloc->alloc());
		dec->setStream(&fsr);

		uint32_t start, end;
		size_t bufferSize = 65536, readSize;
		uint8_t *buffer = new uint8_t[bufferSize];
		while(!stop)
		{
			{
				MutexSession mutex(mutex, true);
				if(tracks.empty())
					break;
				track = tracks.front();
				tracks.pop_front();
			}

			// TODO: Read track information (samples, ...)
			start = static_cast<uint32_t>(getTimestamp((*list)[track][L"Start Time"]));
			end = static_cast<uint32_t>(getTimestamp((*list)[track][L"End Time"]));
			if(start == static_cast<uint32_t>(numeric_limits<int32_t>::max()) || end == static_cast<uint32_t>(numeric_limits<int32_t>::max()))
			{
				// TODO: Error
				continue;
			}

			(*data->callback)(track, 0, 0, 0);

			// TODO: Open written file

			uint64_t section = 0;
			while(!stop)
			{
				if(!dec->readSplit(start, end, buffer, bufferSize, &readSize, &section))
					break;

				// TODO: Write
			}
		}

		data->decAlloc->free(dec);

		InterlockedDecrement(&runningThreads);
	}

	ulong32_t __stdcall RipManager::ripThread(void *param)
	{
		ThreadData *data = static_cast<ThreadData *>(param);
		instance().ripThreadImpl(data);
		
		return 0;
	}
}
