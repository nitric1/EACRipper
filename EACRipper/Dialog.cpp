#include "Defaults.h"

#include "MainController.h"
#include "Dialog.h"
#include "ShortcutKey.h"

using namespace std;

namespace EACRipper
{
	Dialog::Dialog(HWND window)
		: Window(window), dlgData(), isDialogEnded(false), endDialogResult(0)
	{
	}

	Dialog::~Dialog()
	{
	}

	const void *Dialog::getDialogTemplateWithSystemFont()
	{
		dlgData.clear();

		NONCLIENTMETRICSW ncm = {0, };
		ncm.cbSize = CCSIZEOF_STRUCT(NONCLIENTMETRICSW, lfMessageFont); // For Windows under Vista.
		SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);

		HINSTANCE inst = MainController::instance().getInstance();
		HRSRC rsrc = FindResourceW(inst, getDialogName(), RT_DIALOG);
		size_t size = SizeofResource(inst, rsrc);
		HGLOBAL mem = LoadResource(inst, rsrc);
		uint8_t *ptr = static_cast<uint8_t *>(LockResource(mem));
		uint16_t data16;
		uint32_t style;
		wchar_t *stmp;
		size_t tmpsize, prev = 26, next = 26;

		dlgData.assign(ptr, ptr + prev); // dlgVer to cy in DLGTEMPLATEEX: 26 bytes

		auto szOrOrd = [&ptr, &prev, &next, this]() -> void
		{
			prev = next;
			uint16_t *tmp = reinterpret_cast<uint16_t *>(ptr + prev);
			if(*tmp == 0x0000)
				next = prev + 2;
			else if(*tmp == 0xFFFF)
				next = prev + 4;
			else
			{
				wchar_t *stmp = reinterpret_cast<wchar_t *>(tmp);
				next = prev + (wcslen(stmp) + 1) * sizeof(wchar_t);
			}
			dlgData.insert(dlgData.end(), ptr + prev, ptr + next);
		};

		szOrOrd();
		szOrOrd();

		prev = next;
		stmp = reinterpret_cast<wchar_t *>(ptr + prev);
		next = prev + (wcslen(stmp) + 1) * sizeof(wchar_t); // title

		dlgData.insert(dlgData.end(), ptr + prev, ptr + next);

		auto lfHeightToPoint = [](int32_t height) -> uint16_t
		{
			if(height >= 0)
				return static_cast<uint16_t>(height);

			HWND desktop = GetDesktopWindow();
			HDC hdc = GetDC(desktop);
			uint16_t ret = static_cast<uint16_t>(static_cast<double>(-height * 72) / GetDeviceCaps(hdc, LOGPIXELSY) + 0.5);
			ReleaseDC(desktop, hdc);

			return ret;
		};

		prev = next;
		style = *reinterpret_cast<uint32_t *>(ptr + 12);
		if((style & DS_SETFONT) || (style & DS_SHELLFONT))
		{
			data16 = lfHeightToPoint(ncm.lfMessageFont.lfHeight);
			if(data16 < 9)
				data16 = 9; // XXX: Dialog's font size must be least 9pt.
			dlgData.insert(dlgData.end(), reinterpret_cast<uint8_t *>(&data16), reinterpret_cast<uint8_t *>(&data16 + 1));

			prev += 2;
			next = prev + 2;
			dlgData.insert(dlgData.end(), ptr + prev, ptr + next);
			/*dlgData.insert(dlgData.end(), reinterpret_cast<uint8_t *>(&ncm.lfMessageFont.lfWeight),
				reinterpret_cast<uint8_t *>(&ncm.lfMessageFont.lfWeight + 1));*/
			dlgData.push_back(ncm.lfMessageFont.lfItalic);
			dlgData.push_back(ncm.lfMessageFont.lfCharSet);
			next += 2;

			stmp = reinterpret_cast<wchar_t *>(ptr + next);
			prev = next + (wcslen(stmp) + 1) * sizeof(wchar_t);

			tmpsize = (wcslen(ncm.lfMessageFont.lfFaceName) + 1) * sizeof(wchar_t);
			dlgData.insert(dlgData.end(),
				reinterpret_cast<uint8_t *>(ncm.lfMessageFont.lfFaceName),
				reinterpret_cast<uint8_t *>(ncm.lfMessageFont.lfFaceName) + tmpsize);
		}

		size_t rest = 4 - (dlgData.size() % 4);
		if(rest < 4)
		{
			for(; rest > 0; -- rest)
				dlgData.push_back(0);
		}

		rest = 4 - (prev % 4);
		if(rest < 4)
			prev += rest;

		dlgData.insert(dlgData.end(), ptr + prev, ptr + size);

		FreeResource(mem);

		return &*dlgData.begin();
	}

	HWND Dialog::getItemWindow(int32_t id)
	{
		return GetDlgItem(getWindow(), id);
	}

	intptr_t Dialog::showModal(DLGPROC procMessage, int showCommand)
	{
		// From CDialog::DoModal in MFC.

		HWND parentWin = nullptr;
		if(getParent() != nullptr)
			parentWin = getParent()->getWindow();

		const DLGTEMPLATE *tpl = static_cast<const DLGTEMPLATE *>(getDialogTemplateWithSystemFont());
		bool parentEnabled = false;
		if(parentWin != nullptr && IsWindowEnabled(parentWin))
		{
			EnableWindow(parentWin, FALSE);
			parentEnabled = true;
		}

		intptr_t res = -1;
		isDialogEnded = false;
		endDialogResult = -1;
		HWND window = CreateDialogIndirectParamW(MainController::instance().getInstance(), tpl, parentWin, procMessage, 0);
		if(window != nullptr)
		{
			setWindow(window);

			MSG msg;
			while(GetMessageW(&msg, nullptr, 0, 0))
			{
				if(isDialogEnded)
				{
					res = endDialogResult;
					break;
				}

				if(showCommand)
				{
					ShowWindow(window, showCommand);
					UpdateWindow(window);
					showCommand = 0;
				}

				if(msg.message == WM_KEYDOWN && ShortcutKey::instance().processKey(this,
					GetKeyState(VK_CONTROL) < 0, GetKeyState(VK_MENU) < 0, GetKeyState(VK_SHIFT) < 0,
					static_cast<uint32_t>(msg.wParam), ((msg.lParam & 0x01000000) == 0x01000000),
					static_cast<uint32_t>(msg.lParam & 0x0000FFFF)))
				{
					continue;
				}

				if(!IsDialogMessage(window, &msg))
				{
					TranslateMessage(&msg);
					DispatchMessageW(&msg);
				}
			}

			if(!isDialogEnded)
			{
				PostQuitMessage(0);
				return -1;
			}

			SetWindowPos(window, nullptr, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
		}

		if(parentEnabled)
			EnableWindow(parentWin, TRUE);
		if(parentWin != nullptr && GetActiveWindow() == window)
			SetActiveWindow(parentWin);

		DestroyWindow(window);

		setWindow(nullptr);

		return res;

		// return DialogBoxIndirectParamW(MainController::instance().getInstance(), tpl, parentWin, procMessage, 0);
	}

	bool Dialog::endDialog(intptr_t res)
	{
		isDialogEnded = true;
		endDialogResult = res;
		EndDialog(getWindow(), res);
		return true;
	}
}
