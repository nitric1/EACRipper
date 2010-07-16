#include "Defaults.h"

#include "Control.h"

using namespace std;

namespace EACRipper
{
	map<HWND, Control *> Control::controlMap;

	Control::Control(HWND window)
		: Window(window), oldProc(nullptr)
	{
		attach(window);
	}

	Control::~Control()
	{
		detach();
	}

	bool Control::show()
	{
		return ShowWindow(getWindow(), SW_SHOW) != FALSE;
	}

	WNDPROC Control::getOldProc()
	{
		return oldProc;
	}

	bool Control::addControl(HWND window)
	{
		return controlMap.insert(make_pair(window, this)).second;
	}

	void Control::removeControl()
	{
		controlMap.erase(getWindow());
	}

	Control *Control::findControl(HWND window)
	{
		auto it = controlMap.find(window);
		if(it == controlMap.end())
			return nullptr;
		return it->second;
	}

	bool Control::attach(HWND window)
	{
		if(window == nullptr || !detach())
			return false;

		WNDPROC proc = reinterpret_cast<WNDPROC>(GetWindowLongPtrW(window, GWLP_WNDPROC));
		if(proc == oldProc)
			return false;

		if(!addControl(window))
			return false;

		oldProc = proc;
		SetWindowLongPtrW(window, GWLP_WNDPROC, reinterpret_cast<intptr_t>(getWindowProc()));
		setWindow(window);

		return true;
	}

	bool Control::detach()
	{
		if(getWindow() == nullptr)
			return true;

		WNDPROC proc = reinterpret_cast<WNDPROC>(GetWindowLongPtrW(getWindow(), GWLP_WNDPROC));
		if(proc != oldProc)
			return false;

		SetWindowLongPtrW(getWindow(), GWLP_WNDPROC, reinterpret_cast<intptr_t>(oldProc));

		removeControl();

		setWindow(nullptr);
		oldProc = nullptr;

		return true;
	}
}
