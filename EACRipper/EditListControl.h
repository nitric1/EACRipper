#pragma once

#include "Control.h"

namespace EACRipper
{
	class EditListControl : public Control
	{
	public:
		EditListControl();
		virtual ~EditListControl();

	protected:
		virtual WNDPROC getWindowProc();

	private:
		static intptr_t __stdcall windowProc(HWND, unsigned, WPARAM, LPARAM);

	public:
		int32_t addColumn(const std::wstring &, const std::wstring & = std::wstring());
		int32_t insertColumn(int32_t, const std::wstring &, const std::wstring & = std::wstring());
		int32_t addItem(const std::vector<std::wstring> &);
		int32_t insertItem(int32_t, const std::vector<std::wstring> &);
	};
}
