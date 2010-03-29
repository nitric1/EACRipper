#include "Defaults.h"

#include "Utility.h"
#include "ComponentManager.h"

using namespace std;
using namespace std::tr1;

namespace EACRipper
{
	ComponentManager::ComponentManager()
	{
		wstring path = getCurrentDirectoryPath() + L"Components\\";
		wstring findPath = path;
		findPath += L"*.dll";

		HANDLE ff;
		WIN32_FIND_DATA wfd;
		ff = FindFirstFileW(findPath.c_str(), &wfd);
		if(ff == INVALID_HANDLE_VALUE)
			return;

		Component *comp;

		do
		{
			try
			{
				comp = new Component(path + wfd.cFileName);
			}
			catch(exception &)
			{
				continue;
			}
			shared_ptr<Component> compPtr(comp);

			compMap.insert(map<wstring, shared_ptr<Component> >::value_type(wstring(wfd.cFileName), compPtr));
		}
		while(FindNextFileW(ff, &wfd));
	}

	ComponentManager::~ComponentManager()
	{
	}

	const vector<wstring> ComponentManager::components() const
	{
		vector<wstring> v;
		for(map<wstring, shared_ptr<Component> >::const_iterator it = compMap.begin(); it != compMap.end(); ++ it)
		{
			v.push_back(it->first);
		}

		return v;
	}

	shared_ptr<Component> ComponentManager::getComponent(const wstring &name)
	{
		map<wstring, shared_ptr<Component> >::iterator it = compMap.find(name);
		if(it == compMap.end())
			throw(runtime_error("The component does not exist."));
		return it->second;
	}
}
