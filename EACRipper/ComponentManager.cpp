#include "Defaults.h"

#include "ComponentManager.h"

using namespace std;
using namespace std::tr1;

namespace EACRipper
{
	ComponentManager::ComponentManager()
	{
		vector<wchar_t> buffer(512);
		size_t pathlen;

		pathlen = GetModuleFileNameW(NULL, &*buffer.begin(), static_cast<DWORD>(buffer.size()));

		wstring path(buffer.begin(), buffer.begin() + pathlen);
		size_t pos = path.rfind(L'\\');

		if(pos == wstring::npos) // What's happened?
			return;

		path.erase(++ pos);
		path += L"Components\\";

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
			comp = new Component(path + wfd.cFileName);
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
