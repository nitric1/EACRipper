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
	std::shared_ptr<ERDelegate<bool (WindowEventArgs)>> delegateWindowEvent(Func fn)
	{
		return delegate<bool (WindowEventArgs)>(fn);
	}

	template<typename Class, typename Func>
	std::shared_ptr<ERDelegate<bool (WindowEventArgs)>> delegateWindowEvent(Class *p, Func fn)
	{
		return delegate<bool (WindowEventArgs)>(p, fn);
	}

	class Window
	{
	private:
		enum { WM_APP_INVOKE = WM_APP + 101 };

	private:
		std::map<std::wstring, std::vector<std::shared_ptr<ERDelegate<bool (WindowEventArgs)>>>> eventMap;

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

		virtual bool addEventListener(const std::wstring &name, std::shared_ptr<ERDelegate<bool (WindowEventArgs)>> listener)
		{
			eventMap[name].push_back(listener);
			return true;
		}

		virtual void invoke(std::shared_ptr<ERDelegate<void ()>> dg)
		{
			SendMessageW(window, WM_APP_INVOKE, 0, reinterpret_cast<longptr_t>(dg.get()));
		}

	protected:
		bool runEventListener(const std::wstring &name, WindowEventArgs e)
		{
			if(eventMap.find(name) == eventMap.end())
				return true;

			auto &v = eventMap[name];
			for(auto it = v.begin(); it != v.end(); ++ it)
			{
				if(!(*it)->invoke(e))
					return false;
			}

			return true;
		}

		bool procPredefinedMessage(HWND window, unsigned message, uintptr_t wParam, longptr_t lParam)
		{
			switch(message)
			{
			case WM_APP_INVOKE:
				{
					ERDelegate<void ()> *dg = reinterpret_cast<ERDelegate<void ()> *>(lParam);
					(*dg)();
				}
				return true;
			}

			return false;
		}
	};
}