#pragma once

namespace EACRipper
{
	struct WindowEventArgs
	{
		HWND window;
		unsigned message;
		WPARAM wParam;
		LPARAM lParam;
	};
}
