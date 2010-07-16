#include "Defaults.h"

#include "EditListControl.h"

using namespace std;

namespace EACRipper
{
	EditListControl::EditListControl()
		: Control(nullptr)
	{
	}

	EditListControl::~EditListControl()
	{
	}

	WNDPROC EditListControl::getWindowProc()
	{
		return windowProc;
	}

	intptr_t __stdcall EditListControl::windowProc(HWND window, unsigned message, WPARAM wParam, LPARAM lParam)
	{
		EditListControl *selfptr = dynamic_cast<EditListControl *>(findControl(window));
		if(selfptr == nullptr)
			return 0;

		EditListControl &self = *selfptr;

		return CallWindowProcW(self.getOldProc(), window, message, wParam, lParam);
	}

	int32_t EditListControl::addColumn(const wstring &value, const wstring &defVal)
	{
		return 0;
	}

	int32_t EditListControl::insertColumn(int32_t pos, const wstring &value, const wstring &defVal)
	{
		return 0;
	}

	int32_t EditListControl::addItem(const vector<wstring> &vec)
	{
		return 0;
	}

	int32_t EditListControl::insertItem(int32_t pos, const vector<wstring> &vec)
	{
		return 0;
	}
}
