#include "Defaults.h"

#include "MainController.h"
#include "CharsetFileDialog.h"

using namespace std;

namespace EACRipper
{
	CharsetFileDialog::CharsetFileDialog(bool isOpen, Window *owner, const std::wstring &title, const FileDialogFilter &filter, const std::wstring &defExt)
		: FileDialog(isOpen, owner, title, filter, defExt)
	{
		// TODO: Implementation
		// if(cust) addGroup(...);
		// else hook

		IFileDialogCustomize *cust = getCustomize();

		if(cust != nullptr)
		{
			cust->StartVisualGroup(0, L"&Encoding:");
			
			cust->AddComboBox(1);

			cust->EndVisualGroup();
		}
		else
		{
			OPENFILENAMEW &ofn = getOpenFileName();

			ofn.hInstance = MainController::instance().getInstance();
			ofn.lpTemplateName = MAKEINTRESOURCEW(IDD_OPEN_DIALOG);
			ofn.lpfnHook = procCharsetFileDialog;
			ofn.Flags |= OFN_ENABLETEMPLATE | OFN_ENABLEHOOK;
		}
	}

	CharsetFileDialog::~CharsetFileDialog()
	{
	}

	uintptr_t __stdcall CharsetFileDialog::procCharsetFileDialog(HWND window, unsigned message, WPARAM wParam, LPARAM lParam)
	{
		switch(message)
		{
		case WM_INITDIALOG:
			{
			}
			break;

		case WM_SIZE:
			{
				HWND parent, combo, label;
				RECT comborc, labelrc;
				int h;

				parent = GetParent(window);
				combo = GetDlgItem(parent, cmb1);
				label = GetDlgItem(parent, stc2);
				GetWindowRect(combo, &comborc);
				GetWindowRect(label, &labelrc);

				h = comborc.bottom - comborc.top;
				h += 5;

				comborc.top += h;
				comborc.bottom += h;
				labelrc.top += h;
				labelrc.bottom += h;

				combo = GetDlgItem(window, IDC_ENCODING);
				label = GetDlgItem(window, IDC_ENCODING_LABEL);
				MoveWindow(combo, comborc.left, comborc.top, comborc.right - comborc.left, comborc.bottom - comborc.top, TRUE);
				MoveWindow(label, labelrc.left, labelrc.top, labelrc.right - labelrc.left, labelrc.bottom - labelrc.top, TRUE);
			}
			break;
		}

		return 0;
	}

	void CharsetFileDialog::convertRectToClient(HWND window, RECT *p)
	{
		POINT pt;
		pt.x = p->left;
		pt.y = p->top;
		ScreenToClient(window, &pt);
		p->left = pt.x;
		p->top = pt.y;

		pt.x = p->right;
		pt.y = p->bottom;
		ScreenToClient(window, &pt);
		p->right = pt.x;
		p->bottom = pt.y;
	}

	bool CharsetFileDialog::show()
	{
		return FileDialog::show();
	}

	const string &CharsetFileDialog::getCharset() const
	{
		return charset;
	}
}
