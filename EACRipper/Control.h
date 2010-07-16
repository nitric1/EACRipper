#pragma once

#include "Window.h"

namespace EACRipper
{
	class Control : public Window
	{
	private:
		WNDPROC oldProc;
		static std::map<HWND, Control *> controlMap;

	protected:
		explicit Control(HWND = nullptr);
		virtual ~Control() = 0;

	protected:
		virtual WNDPROC getWindowProc() = 0;
		WNDPROC getOldProc();

	private:
		bool addControl(HWND);
		void removeControl();

	protected:
		static Control *findControl(HWND);

	public:
		virtual bool show();
		bool attach(HWND);
		bool detach();
	};
}
