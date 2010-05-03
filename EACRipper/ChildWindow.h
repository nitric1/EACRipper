#pragma once

#include "Window.h"

namespace EACRipper
{
	class ChildWindow : public Window
	{
	private:
		Window *parent;

	protected:
		explicit ChildWindow(HWND iwindow = nullptr) : Window(iwindow), parent(nullptr) {}
		virtual ~ChildWindow() = 0 {}

	public:
		Window *getParent() { return parent; }
		void setParent(Window *iparent) { parent = iparent; }
		virtual bool showWithParent(Window *parent) { setParent(parent); return show(); }
	};
}
