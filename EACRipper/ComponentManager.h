#pragma once

#include "Singleton.h"
#include "Component.h"

namespace EACRipper
{
	class ComponentManager : public Singleton<ComponentManager>
	{
	private:
		std::map<std::wstring, std::shared_ptr<Component> > compMap;

	private:
		ComponentManager();
		~ComponentManager();

	public:
		const std::vector<std::wstring> components() const;
		std::shared_ptr<Component> getComponent(const std::wstring &);

		friend class Singleton<ComponentManager>;
	};
}
