#include "Defaults.h"
#include "RipManager.h"

using namespace std;

namespace EACRipper
{
	RipManager::RipManager()
		: processorCount(1), runningThreads(0), stop(false)
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);

		processorCount = si.dwNumberOfProcessors;
	}

	RipManager::~RipManager()
	{
		stopRip();
	}

	bool RipManager::startRip(const TrackList &ilist, std::shared_ptr<RipCallbackDelegate> progressCallback)
	{
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

		for(uint32_t i = 0; i < processorCount; ++ i)
		{
			InterlockedIncrement(&runningThreads);
			threadData[i].threadId = i;
			threads[i] = CreateThread(NULL, 0, ripThread, &threadData[i], 0, nullptr);
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
		return true;
	}

	bool RipManager::isRipping()
	{
		return runningThreads > 0;
	}

	ulong32_t __stdcall RipManager::ripThread(void *param)
	{
		ThreadData *data = static_cast<ThreadData *>(param);
		RipManager &self = instance();

		while(!self.stop)
		{
		}

		InterlockedDecrement(&self.runningThreads);
		
		return 0;
	}
}
