#include "Defaults.h"
#include "RipManager.h"

using namespace std;

namespace EACRipper
{
	RipManager::RipManager()
		: processorCount(1)
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);

		processorCount = si.dwNumberOfProcessors;
	}

	RipManager::~RipManager()
	{
	}

	void RipManager::openCuesheet(const wstring &path)
	{
	}

	bool RipManager::startRip()
	{
		return false;
	}

	bool RipManager::terminateRip()
	{
		return false;
	}
}
