#pragma once

namespace EACRipper
{
	class Window;

	struct WindowEventArgs
	{
		Window *window;
		HWND windowHandle;
		unsigned message;
		WPARAM wParam;
		LPARAM lParam;
	};

	class EventDelegate
	{
	public:
		virtual bool run(WindowEventArgs e) = 0;
		bool operator ()(WindowEventArgs e) { return run(e); }
	};

	template<typename Func>
	class EventDelegateToFunction : public EventDelegate
	{
	private:
		Func fn;

	private:
		explicit EventDelegateToFunction(Func ifn) : fn(ifn) {}
		EventDelegateToFunction(const EventDelegateToFunction<Func> &dg) : fn(dg.fn) {}

	public:
		virtual bool run(WindowEventArgs e) { return fn(e); }

		template<typename Func>
		friend std::tr1::shared_ptr<EventDelegateToFunction<Func>> delegateEvent(Func);
	};

	template<typename Class, typename Func>
	class EventDelegateToMemberFunction : public EventDelegate
	{
	private:
		Class *p;
		Func fn;

	private:
		explicit EventDelegateToMemberFunction(Class *ip, Func ifn) : p(ip), fn(ifn) {}
		EventDelegateToMemberFunction(const EventDelegateToMemberFunction<Class, Func> &dg) : p(dg.p), fn(dg.fn) {}

	public:
		virtual bool run(WindowEventArgs e) { return (p->*fn)(e); }

		template<typename Class, typename Func>
		friend std::tr1::shared_ptr<EventDelegateToMemberFunction<Class, Func>> delegateEvent(Class *, Func);
	};

	template<typename Func>
	std::tr1::shared_ptr<EventDelegateToFunction<Func>> delegateEvent(Func fn)
	{
		typedef decltype(delegateEvent(fn)) returnType;
		return returnType(new EventDelegateToFunction<Func>(fn));
	}

	template<typename Class, typename Func>
	std::tr1::shared_ptr<EventDelegateToMemberFunction<Class, Func>> delegateEvent(Class *p, Func fn)
	{
		typedef decltype(delegateEvent(p, fn)) returnType;
		return returnType(new EventDelegateToMemberFunction<Class, Func>(p, fn));
	}

	class Window
	{
	private:
		std::map<std::wstring, std::vector<std::tr1::shared_ptr<EventDelegate>>> eventMap;

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

		virtual bool addEventListener(const std::wstring &name, std::tr1::shared_ptr<EventDelegate> listener)
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