#include "Defaults.h"

#include "EditListControl.h"

using namespace std;

namespace EACRipper
{
	EditListControl::EditListControl()
		: Control(nullptr), columns(0)
	{
	}

	EditListControl::~EditListControl()
	{
	}

	WNDPROC EditListControl::getWindowProc()
	{
		return windowProc;
	}

	longptr_t __stdcall EditListControl::windowProc(HWND window, unsigned message, WPARAM wParam, LPARAM lParam)
	{
		EditListControl *selfptr = dynamic_cast<EditListControl *>(findControl(window));
		if(selfptr == nullptr)
			return 0;

		EditListControl &self = *selfptr;

		return CallWindowProcW(self.getOldProc(), window, message, wParam, lParam);
	}

	int32_t EditListControl::addColumn(const wstring &value, int32_t width, int32_t align, const wstring &defVal)
	{
		return insertColumn(columns, value, width, align, defVal);
	}

	int32_t EditListControl::insertColumn(int32_t pos, const wstring &value, int32_t width, int32_t align, const wstring &defVal)
	{
		LVCOLUMNW lvc;
		lvc.cx = width;
		lvc.fmt = align;
		lvc.pszText = const_cast<wchar_t *>(value.c_str());
		lvc.mask = LVCF_WIDTH | LVCF_FMT | LVCF_TEXT;
		int32_t res = static_cast<int32_t>(SendMessageW(getWindow(), LVM_INSERTCOLUMNW, static_cast<WPARAM>(pos), reinterpret_cast<LPARAM>(&lvc)));
		if(res >= 0)
			++ columns;
		return res;
	}

	int32_t EditListControl::addItem(const vector<wstring> &vec)
	{
		return 0;
	}

	int32_t EditListControl::insertItem(int32_t pos, const vector<wstring> &vec)
	{
		return 0;
	}

	void EditListControl::clear()
	{
		SendMessageW(getWindow(), LVM_DELETEALLITEMS, 0, 0);
	}

	bool EditListControl::attach(HWND window)
	{
		HWND header = reinterpret_cast<HWND>(SendMessageW(window, LVM_GETHEADER, 0, 0));
		if(header == nullptr)
			return false;

		int32_t count = static_cast<int32_t>(SendMessageW(header, HDM_GETITEMCOUNT, 0, 0));
		if(count < 0)
			return false;

		if(!Control::attach(window))
			return false;

		columns = count;
		return true;
	}

	bool EditListControl::detach()
	{
		if(!Control::detach())
			return false;

		columns = 0;
		return true;
	}
}
