#include "Defaults.h"

#include "MusicCoderManager.h"

using namespace std;

namespace EACRipper
{
	MusicCoderManager::~MusicCoderManager()
	{
	}

	const vector<pair<wstring, MusicCoderManager::CoderType> > MusicCoderManager::coders() const
	{
		vector<pair<wstring, CoderType> > v;
		for(auto it = coderMap.begin(); it != coderMap.end(); ++ it)
		{
			v.push_back(it->first);
		}

		return v;
	}

	bool MusicCoderManager::addCoder(const wstring &name, CoderType type, IERAllocator *alloc)
	{
		return coderMap.insert(map<pair<wstring, CoderType>, IERAllocator *>::value_type(make_pair(name, type), alloc)).second;
	}

	IERAllocator *MusicCoderManager::getCoder(const wstring &name, CoderType type)
	{
		auto it = coderMap.find(make_pair(name, type));
		if(it == coderMap.end())
			throw(runtime_error("The coder does not exist."));
		return it->second;
	}
}
