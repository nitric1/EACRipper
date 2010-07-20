#pragma once

#include "Window.h"

namespace EACRipper
{
	class Dialog : public Window
	{
	private:
		std::vector<uint8_t> dlgData;

	protected:
		explicit Dialog(HWND = nullptr);
		virtual ~Dialog() = 0;

	protected:
		virtual const wchar_t *getDialogName() = 0;
		virtual const void *getDialogTemplateWithSystemFont();
		virtual bool show(DLGPROC);
	};
};
