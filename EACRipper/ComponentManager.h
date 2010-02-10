#pragma once

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>

#include "Singleton.h"
#include "Component.h"

namespace EACRipper
{
	class ComponentManager : public Singleton<ComponentManager>
	{
	public:
		static const std::string COMPONENT_TYPE_MUSIC;
		static const std::string COMPONENT_TYPE_ARCHIVE;

	private:
		std::map<std::wstring, std::tr1::shared_ptr<Component> > comp;

	private:
		ComponentManager();
		~ComponentManager();

	public:
		const std::vector<std::wstring> components(const std::string &) const;
		std::tr1::shared_ptr<Component> getComponent(const std::wstring &);

		friend class Singleton<ComponentManager>;
	};
}
