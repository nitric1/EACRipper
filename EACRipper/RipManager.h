#pragma once

#include "IERComponentMusic.h"

#include "Singleton.h"
#include "TrackList.h"
#include "CoderManager.h"

namespace EACRipper
{
	class RipCallbackDelegate
	{
	public:
		virtual bool run(size_t, uint64_t, uint64_t, int32_t) = 0;
		bool operator ()(size_t track, uint64_t totalSamples, uint64_t currentSamples, int32_t errorCode)
		{
			return run(track, totalSamples, currentSamples, errorCode);
		}
	};

	template<typename Func>
	class RipCallbackFunctionDelegate : public RipCallbackDelegate
	{
	private:
		Func fn;

	private:
		explicit RipCallbackFunctionDelegate(Func ifn) : fn(ifn) {}

	public:
		virtual bool run(size_t track, uint64_t totalSamples, uint64_t currentSamples, int32_t errorCode)
		{
			return fn(track, totalSamples, currentSamples, errorCode);
		}

		template<typename Func>
		friend std::shared_ptr<RipCallbackFunctionDelegate<Func>> delegateRipCallback(Func);
	};

	template<typename Class, typename Func>
	class RipCallbackMemberFunctionDelegate : public RipCallbackDelegate
	{
	private:
		Class *p;
		Func fn;

	private:
		explicit RipCallbackMemberFunctionDelegate(Class *ip, Func ifn) : p(ip), fn(ifn) {}

	public:
		virtual bool run(size_t track, uint64_t totalSamples, uint64_t currentSamples, int32_t errorCode)
		{
			return (p->*fn)(track, totalSamples, currentSamples, errorCode);
		}

		template<typename Class, typename Func>
		friend std::shared_ptr<RipCallbackMemberFunctionDelegate<Class, Func>> delegateRipCallback(Class *, Func);
	};

	template<typename Func>
	std::shared_ptr<RipCallbackFunctionDelegate<Func>> delegateRipCallback(Func fn)
	{
		typedef decltype(delegateRipCallback(fn)) returnType;
		return returnType(new RipCallbackFunctionDelegate<Func>(fn));
	}

	template<typename Class, typename Func>
	std::shared_ptr<RipCallbackMemberFunctionDelegate<Class, Func>> delegateRipCallback(Class *p, Func fn)
	{
		typedef decltype(delegateRipCallback(p, fn)) returnType;
		return returnType(new RipCallbackMemberFunctionDelegate<Class, Func>(p, fn));
	}

	class RipManager : public Singleton<RipManager>
	{
	private:
		struct ThreadData
		{
			uint32_t threadId;
			std::shared_ptr<RipCallbackDelegate> callback;
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
		bool startRip(const TrackList &, std::shared_ptr<RipCallbackDelegate>);
		bool stopRip();
		bool isRipping();

		static ulong32_t __stdcall ripThread(void *);

		friend class Singleton<RipManager>;
	};
}
