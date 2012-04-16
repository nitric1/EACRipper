#pragma once

#include "Delegate.h"

namespace EACRipper
{
	class Window;

	struct WindowEventArgs
	{
		Window *window;
		HWND windowHandle;
		uint32_t message;
		uintptr_t wParam;
		longptr_t lParam;
	};

	template<typename Func>
	std::shared_ptr<ERDelegate<bool (const WindowEventArgs &)>> delegateWindowEvent(Func fn)
	{
		return delegate<bool (const WindowEventArgs &)>(fn);
	}

	template<typename Class, typename Func>
	std::shared_ptr<WindowEventMemberFunctionDelegate<Class, Func>> delegateWindowEvent(Class *p, Func fn)
	{
		return delegate<bool (const WindowEventArgs &)>(p, fn);
	}

	class Window
	{
	private:
		std::map<std::wstring, std::vector<std::shared_ptr<WindowEventDelegate>>> eventMap;

	private:
		HWND window;
		int showCommand;
		Window *parent;

	protected:
		explicit Window(HWND iwindow = nullptr) : window(iwindow) {}
		virtual ~Window() = 0 {}

	public:
		virtual int getShowStatus() const
		{
			return showCommand;
		}

		virtual void setShowStatus(int ishowCommand)
		{
			showCommand = ishowCommand;
		}

		virtual bool show() = 0;

		virtual HWND getWindow() const
		{
			return window;
		}

		virtual void setWindow(HWND iwindow)
		{
			window = iwindow;
		}

		Window *getParent()
		{
			return parent;
		}

		const Window *getParent() const
		{
			return parent;
		}

		void setParent(Window *iparent)
		{
			parent = iparent;
		}

		virtual bool showWithParent(Window *parent)
		{
			setParent(parent);
			return show();
		}

		virtual bool addEventListener(const std::wstring &name, std::shared_ptr<WindowEventDelegate> listener)
		{
			eventMap[name].push_back(listener);
			return true;
		}

	protected:
		bool runEventListener(const std::wstring &name, WindowEventArgs e)
		{
			if(eventMap.find(name) == eventMap.end())
				return true;

			auto &v = eventMap[name];
			for(auto it = v.begin(); it != v.end(); ++ it)
			{
				if(!(*it)->run(e))
					return false;
			}

			return true;
		}
	};
}