#pragma once

#include <map>
#include <vector>
#include <string>

#include "IERAllocator.h"

#include "Singleton.h"

namespace EACRipper
{
	class MusicCoderManager : public Singleton<MusicCoderManager>
	{
	public:
		typedef enum _CoderType
		{
			Decoder,
			InCueDecoder,
			Encoder
		} CoderType;

	public:
		std::map<std::wstring, std::pair<CoderType, IERAllocator *> > coderMap;

	private:
		~MusicCoderManager();

	public:
		const std::vector<std::pair<std::wstring, CoderType> > coders() const;
		bool addCoder(const std::wstring &, CoderType, IERAllocator *);
		IERAllocator *getCoder(const std::wstring &);

		friend class Singleton<MusicCoderManager>;
	};
}
