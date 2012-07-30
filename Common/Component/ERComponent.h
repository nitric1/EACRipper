#pragma once

#ifndef _INCLUDE_ERCOMPONENT_
#define _INCLUDE_ERCOMPONENT_

#include <cinttypes>

#include <limits>

#include <windows.h>

#include "ERTypes.h"
#include "EREndian.h"
#include "ERException.h"
#include "ERUUID.h"
#include "IERString.h"
#include "IERStream.h"
#include "IERFile.h"
#include "IERApplication.h"
#include "IERComponentArchive.h"
#include "IERComponentEntrypoint.h"
#include "IERComponentMusic.h"
#include "IERComponentInfo.h"

#if defined(_WINDOWS) && defined(_USRDLL)
extern "C" __declspec(dllexport) ERException *invokeCdecl(uintptr_t *, uintptr_t *, void *, uint32_t, ...);
extern "C" __declspec(dllexport) ERException *invokeThiscall(uintptr_t *, uintptr_t *, void *, void *, uint32_t, ...);
extern "C" __declspec(dllexport) void initComponent(IERApplication *);
extern "C" __declspec(dllexport) void uninitComponent();
#endif

#include "Component/Definition.h"
#include "Component/Invoke.h"
#include "Component/Info.h"
#include "Component/Factory.h"
#include "Component/Allocator.h"
#include "Component/Archive.h"
#include "Component/Music.h"

#define DECLARE_COMPONENT(name, version) \
	namespace ERComponentGlobal { \
		const wchar_t *componentName = name; \
		const wchar_t *componentVersion = version; \
		IERComponentEntrypoint *componentEntry = nullptr; \
		IERComponentInfo *info = nullptr; \
	}

#endif
