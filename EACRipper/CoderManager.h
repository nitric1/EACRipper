#pragma once

#include "IERAllocator.h"

#include "Singleton.h"

namespace EACRipper
{
	template<typename Key>
	class CoderManager
	{
	private:
		std::map<Key, IERAllocator *> coderMap;

	public:
		std::vector<Key> coders() const
		{
			vector<Key> ve;
			std::for_each(coderMap.begin(), coderMap.end(), [&ve](const std::pair<Key, IERAllocator *> &elem) { ve.push_back(elem.first); });
			return ve;
		}
		bool addCoder(const Key &key, IERAllocator *alloc)
		{
			return coderMap.insert(std::map<Key, IERAllocator *>::value_type(key, alloc)).second;
		}
		IERAllocator *getCoder(const Key &key)
		{
			auto it = coderMap.find(key);
			if(it == coderMap.end())
				return nullptr;
			return it->second;
		}
	};

	class ArchiveCoderManager : public CoderManager<std::wstring>, public Singleton<ArchiveCoderManager>
	{
	private:
		~ArchiveCoderManager();

		friend class Singleton<ArchiveCoderManager>;
	};

	class MusicCoderManager : public CoderManager<std::pair<std::wstring, int_fast32_t>>, public Singleton<MusicCoderManager>
	{
	public:
		typedef enum _CoderType
		{
			Decoder,
			InCueDecoder,
			Encoder
		} CoderType;

	private:
		~MusicCoderManager();

		friend class Singleton<MusicCoderManager>;
	};
}
