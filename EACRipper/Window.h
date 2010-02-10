#pragma once

#include <map>
#include <vector>
#include <string>

#include "WindowEventArgs.h"

namespace EACRipper
{
	template<typename Controller>
	class Window
	{
	public:
		typedef bool (Controller::*EventListener)(WindowEventArgs);

	private:
		Controller *controller;
		std::map<std::string, std::vector<EventListener> > eventMap;

	protected:
		Window() : controller(NULL) {}
		virtual ~Window() = 0 {}

	public:
		Controller *getOwner()
		{
			return controller;
		}

		void setOwner(Controller *cont)
		{
			controller = cont;
		}

	public:
		virtual bool addEventListener(const std::string &name, EventListener listener)
		{
			if(controller == NULL)
				return false;
			eventMap[name].push_back(listener);
			return true;
		}

	protected:
		bool runEventListener(const std::string &name, WindowEventArgs e)
		{
			if(controller == NULL)
				return false;
			else if(eventMap.find(name) == eventMap.end())
				return true;

			std::vector<EventListener> &v = eventMap[name];
			for(std::vector<EventListener>::iterator it = v.begin(); it != v.end(); ++ it)
			{
				if(!(controller->*(*it))(e))
					return false;
			}

			return true;
		}
	};
}
