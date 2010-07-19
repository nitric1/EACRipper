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
		virtual const uint8_t *getDialogTemplateWithSystemFont();
		virtual bool show(DLGPROC);
	};
};
