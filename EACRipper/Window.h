#pragma once

#include "WindowBase.h"

namespace EACRipper
{
	struct WindowEventArgs
	{
		HWND window;
		unsigned message;
		WPARAM wParam;
		LPARAM lParam;
	};

	template<typename Controller>
	class Window : public WindowBase
	{
	public:
		typedef bool (Controller::*EventListener)(WindowEventArgs);

	private:
		Controller *controller;
		std::map<std::wstring, std::vector<EventListener> > eventMap;

	protected:
		explicit Window(HWND iwindow = nullptr) : WindowBase(iwindow), controller(nullptr) {}
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

		virtual bool show() = 0;

	public:
		virtual bool addEventListener(const std::wstring &name, EventListener listener)
		{
			if(controller == nullptr)
				return false;
			eventMap[name].push_back(listener);
			return true;
		}

	protected:
		bool runEventListener(const std::wstring &name, WindowEventArgs e)
		{
			if(controller == nullptr)
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