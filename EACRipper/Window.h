#pragma once

#include "WindowBase.h"
#include "WindowEventArgs.h"

namespace EACRipper
{
	template<typename Controller>
	class Window : public WindowBase
	{
	public:
		typedef bool (Controller::*EventListener)(WindowEventArgs);

	private:
		Controller *controller;
		std::map<std::wstring, std::vector<EventListener> > eventMap;

	protected:
		explicit Window(HWND iwindow = NULL) : WindowBase(iwindow), controller(NULL) {}
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
		virtual bool addEventListener(const std::wstring &name, EventListener listener)
		{
			if(controller == NULL)
				return false;
			eventMap[name].push_back(listener);
			return true;
		}

	protected:
		bool runEventListener(const std::wstring &name, WindowEventArgs e)
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