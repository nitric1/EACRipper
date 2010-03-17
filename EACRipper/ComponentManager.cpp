#include "Defaults.h"

#include "ComponentManager.h"

using namespace std;
using namespace std::tr1;

namespace EACRipper
{
	const wstring ComponentManager::COMPONENT_TYPE_MUSIC = L"music";
	const wstring ComponentManager::COMPONENT_TYPE_ARCHIVE = L"archive";

	ComponentManager::ComponentManager()
	{
		// TODO: Load components.
	}

	ComponentManager::~ComponentManager()
	{
	}

	const vector<wstring> ComponentManager::components(const string &type) const
	{
		vector<wstring> v;
		for(map<wstring, shared_ptr<Component> >::const_iterator it = comp.begin(); it != comp.end(); ++ it)
		{
			v.push_back(it->first);
		}

		return v;
	}

	shared_ptr<Component> ComponentManager::getComponent(const wstring &name)
	{
		map<wstring, shared_ptr<Component> >::iterator it = comp.find(name);
		if(it == comp.end())
			throw(runtime_error("The component does not exist."));
		return it->second;
	}
}
