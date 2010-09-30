#pragma once

#include "Window.h"

namespace EACRipper
{
	class Dialog : public Window
	{
	private:
		enum { WM_APP_ENDDIALOG = WM_APP + 100 };

	private:
		std::vector<uint8_t> dlgData;
		bool isDialogEnded;
		intptr_t endDialogResult;

	protected:
		explicit Dialog(HWND = nullptr);
		virtual ~Dialog() = 0;

	protected:
		virtual const wchar_t *getDialogName() = 0;
		virtual const void *getDialogTemplateWithSystemFont();
		virtual HWND getItemWindow(int32_t);

		intptr_t showModal(DLGPROC, int = SW_SHOW);
		bool endDialog(intptr_t);
	};
};
