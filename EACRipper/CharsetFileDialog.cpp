#include "Defaults.h"

#include "CharsetFileDialog.h"

namespace EACRipper
{
	CharsetFileDialog::CharsetFileDialog(bool isOpen, Window *owner, const std::wstring &title, const FileDialogFilter &filter, const std::wstring &defExt)
		: FileDialog(isOpen, owner, title, filter, defExt)
	{
		// TODO: Implementation
		// if(cust) addGroup(...);
		// else hook
	}

	CharsetFileDialog::~CharsetFileDialog()
	{
	}

	bool CharsetFileDialog::show()
	{
		return FileDialog::show();
	}
}
