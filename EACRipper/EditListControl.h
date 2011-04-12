#pragma once

#include "Control.h"

namespace EACRipper
{
	class EditListControl : public Control
	{
	public:
		enum
		{
			ALIGN_LEFT,
			ALIGN_RIGHT,
			ALIGN_CENTER
		};

	private:
		int32_t columns;

	public:
		EditListControl();
		virtual ~EditListControl();

	protected:
		virtual WNDPROC getWindowProc();

	private:
		static longptr_t __stdcall windowProc(HWND, unsigned, WPARAM, LPARAM);

	public:
		virtual int32_t addColumn(const std::wstring &, int32_t, int32_t = ALIGN_LEFT, bool = false, const std::wstring & = std::wstring());
		virtual int32_t insertColumn(int32_t, const std::wstring &, int32_t, int32_t = ALIGN_LEFT, bool = false, const std::wstring & = std::wstring());
		virtual int32_t addItem(const std::vector<std::wstring> &);
		virtual int32_t insertItem(int32_t, const std::vector<std::wstring> &);
		virtual void clear();

	public:
		virtual bool attach(HWND);
		virtual bool detach();
	};
}
