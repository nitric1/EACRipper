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

	int32_t EditListControl::addColumn(const wstring &value, int32_t width, int32_t align, bool readonly, const wstring &defVal)
	{
		return insertColumn(columns, value, width, align, readonly, defVal);
	}

	int32_t EditListControl::insertColumn(int32_t pos, const wstring &value, int32_t width, int32_t align, bool readonly, const wstring &defVal)
	{
		LVCOLUMNW lvc = {0, };
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
		return insertItem(static_cast<int32_t>(SendMessageW(getWindow(), LVM_GETITEMCOUNT, 0, 0)), vec);
	}

	int32_t EditListControl::insertItem(int32_t pos, const vector<wstring> &vec)
	{
		HWND list = getWindow();
		LVITEMW lvi = {0, };
		lvi.mask = LVIF_TEXT;
		lvi.iItem = ListView_GetItemCount(list);
		int32_t order = 0, row = 0;
		for_each(vec.begin(), vec.end(), [&](const wstring &str)
		{
			lvi.iSubItem = order ++;
			lvi.pszText = const_cast<wchar_t *>(str.c_str());
			if(order == 1)
				row = static_cast<int32_t>(SendMessageW(list, LVM_INSERTITEMW, 0, reinterpret_cast<LPARAM>(&lvi)));
			else
				SendMessageW(list, LVM_SETITEMW, 0, reinterpret_cast<LPARAM>(&lvi));
		});
		ListView_SetCheckState(list, lvi.iItem, TRUE);
		return row;
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
