#pragma once

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

	class WindowEventDelegate
	{
	public:
		virtual bool run(WindowEventArgs e) = 0;
		bool operator ()(WindowEventArgs e) { return run(e); }
	};

	template<typename Func>
	class WindowEventFunctionDelegate : public WindowEventDelegate
	{
	private:
		Func fn;

	private:
		explicit WindowEventFunctionDelegate(Func ifn) : fn(ifn) {}

	public:
		virtual bool run(WindowEventArgs e) { return fn(e); }

		template<typename Func>
		friend std::shared_ptr<WindowEventFunctionDelegate<Func>> delegateWindowEvent(Func);
	};

	template<typename Class, typename Func>
	class WindowEventMemberFunctionDelegate : public WindowEventDelegate
	{
	private:
		Class *p;
		Func fn;

	private:
		explicit WindowEventMemberFunctionDelegate(Class *ip, Func ifn) : p(ip), fn(ifn) {}

	public:
		virtual bool run(WindowEventArgs e) { return (p->*fn)(e); }

		template<typename Class, typename Func>
		friend std::shared_ptr<WindowEventMemberFunctionDelegate<Class, Func>> delegateWindowEvent(Class *, Func);
	};

	template<typename Func>
	std::shared_ptr<WindowEventFunctionDelegate<Func>> delegateWindowEvent(Func fn)
	{
		typedef decltype(delegateWindowEvent(fn)) returnType;
		return returnType(new WindowEventFunctionDelegate<Func>(fn));
	}

	template<typename Class, typename Func>
	std::shared_ptr<WindowEventMemberFunctionDelegate<Class, Func>> delegateWindowEvent(Class *p, Func fn)
	{
		typedef decltype(delegateWindowEvent(p, fn)) returnType;
		return returnType(new WindowEventMemberFunctionDelegate<Class, Func>(p, fn));
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