#include "Defaults.h"

#include <windows.h>

#include "Main.h"

namespace EACRipper
{
	Main::Main()
	{
	}

	Main::~Main()
	{
	}

	int Main::run(HINSTANCE inst)
	{
		return 0;
	}
}

int __stdcall wWinMain(HINSTANCE inst, HINSTANCE, LPWSTR, int)
{
	return EACRipper::Main::instance().run(inst);
}
