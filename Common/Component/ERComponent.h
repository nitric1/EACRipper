#pragma once

#ifndef _INCLUDE_ERCOMPONENT_
#define _INCLUDE_ERCOMPONENT_

#define __STDC_FORMAT_MACROS
#include <cinttypes>

#include <windows.h>

#define EACRIPPER_COMPONENT_SDK_VERSION L"2.0.0"

#include "IERApplication.h"
#include "IERComponentEntrypoint.h"
#include "IERComponentInfo.h"
#include "IERStreamReader.h"
#include "IERStreamWriter.h"
#include "ERUUID.h"

#if defined(_WINDOWS) && defined(_USRDLL)
extern "C" __declspec(dllexport) void initComponent(IERApplication *);
extern "C" __declspec(dllexport) void uninitComponent();
#endif

#include "Component/Info.h"
#include "Component/Factory.h"
#include "Component/Archive.h"
#include "Component/Music.h"

#define DECLARE_COMPONENT(name, version) \
	namespace ERComponentGlobal { \
		const wchar_t *componentName = name; \
		const wchar_t *componentVersion = version; \
		IERComponentEntrypoint *componentEntry = NULL; \
	}

#if defined(_WINDOWS) && defined(_USRDLL)
extern "C" __declspec(dllexport) void initComponent(IERApplication *app)
{
	ERComponent::ApplicationInfo::instance().setApp(app);
	if(ERComponentGlobal::componentEntry != NULL)
		ERComponentGlobal::componentEntry->onInit();
}

extern "C" __declspec(dllexport) void uninitComponent()
{
	if(ERComponentGlobal::componentEntry != NULL)
		ERComponentGlobal::componentEntry->onUninit();
}
#endif

#endif
