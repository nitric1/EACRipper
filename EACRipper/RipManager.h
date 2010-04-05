#pragma once

#include "Singleton.h"

namespace EACRipper
{
	class RipManager : public Singleton<RipManager>
	{
	private:
		unsigned processorCount;
		
	private:
		RipManager();
		~RipManager();

	public:
		void openCuesheet(const std::wstring &);
		bool startRip();
		bool terminateRip();
	};
}
