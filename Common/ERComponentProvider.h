#pragma once

#include "../Common/IERComponentInfo.h"

#if defined(_WINDOWS) && defined(_USRDLL)
#include <windows.h>

extern "C" __declspec(dllexport) void initComponent(IERApplication *);
extern "C" __declspec(dllexport) void uninitComponent(IERApplication *);
#endif

#ifndef _INC_WINDOWS
#error Include <windows.h> before include this header!
#endif

#define DECLARE_COMPONENT(name, cls) \
	;

class ERServiceFactory
{
};
