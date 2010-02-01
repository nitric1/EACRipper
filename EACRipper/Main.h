#pragma once

#include "Singleton.h"

namespace EACRipper
{
	class Main : public Singleton<Main>
	{
	protected:
		Main();
		~Main();

	protected:
		static BOOL __stdcall DialogProc(HWND, unsigned, WPARAM, LPARAM);

	public:
		int run(HINSTANCE);

		friend class Singleton<Main>;
	};
}
