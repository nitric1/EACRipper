#pragma once

namespace EACRipper
{
	class WindowBase
	{
	protected:
		HWND window;

	public:
		WindowBase(HWND iwindow) : window(iwindow) {}
		virtual ~WindowBase() = 0 {}

	public:
		virtual HWND getWindow()
		{
			return window;
		}
	};
}
