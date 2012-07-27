#pragma once

#include "IERComponentMusic.h"

#include "CoderManager.h"
#include "Delegate.h"
#include "Singleton.h"
#include "TrackList.h"

namespace EACRipper
{
	template<typename Func>
	std::shared_ptr<ERDelegate<bool (size_t, uint64_t, uint64_t, int32_t)>> delegateRipCallback(Func fn)
	{
		return delegate<bool (size_t, uint64_t, uint64_t, int32_t)>(fn);
	}

	template<typename Class, typename Func>
	std::shared_ptr<ERDelegate<bool (size_t, uint64_t, uint64_t, int32_t)>> delegateRipCallback(Class *p, Func fn)
	{
		return delegate<bool (size_t, uint64_t, uint64_t, int32_t)>(p, fn);
	}

	class RipManager : public Singleton<RipManager>
	{
	private:
		struct ThreadData
		{
			uint32_t threadId;
			std::shared_ptr<ERDelegate<bool (size_t, uint64_t, uint64_t, int32_t)>> callback;
			IERAllocator *decAlloc;
		};

	private:
		uint32_t processorCount;
		std::deque<size_t> tracks;
		std::shared_ptr<TrackList> list;
		std::vector<HANDLE> threads;
		std::vector<ThreadData> threadData;
		volatile uint32_t runningThreads;
		HANDLE mutex;
		volatile bool stop;
		
	private:
		RipManager();
		~RipManager();

	public:
		bool startRip(const TrackList &, std::shared_ptr<ERDelegate<bool (size_t, uint64_t, uint64_t, int32_t)>>);
		bool stopRip();
		bool isRipping();

		void ripThreadImpl(ThreadData *);

		static ulong32_t __stdcall ripThread(void *);

		friend class Singleton<RipManager>;
	};
}
