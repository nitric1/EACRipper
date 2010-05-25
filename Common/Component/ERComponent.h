#pragma once

#ifndef _INCLUDE_ERCOMPONENT_
#define _INCLUDE_ERCOMPONENT_

#include <cinttypes>

#include <windows.h>

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
extern "C" __declspec(dllexport) void initComponent(IERApplication *);
extern "C" __declspec(dllexport) void uninitComponent();
#endif

#include "Component/Definition.h"
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

#if defined(_WINDOWS) && defined(_USRDLL)
extern "C" __declspec(dllexport) void initComponent(IERApplication *app)
{
	ERComponent::ApplicationInfo::instance().setApp(app);
	ERComponentGlobal::info = new ERComponent::Info();
	app->setInfo(ERComponentGlobal::info);
	if(ERComponentGlobal::componentEntry != nullptr)
		ERComponentGlobal::componentEntry->onInit();
}

extern "C" __declspec(dllexport) void uninitComponent()
{
	if(ERComponentGlobal::componentEntry != nullptr)
		ERComponentGlobal::componentEntry->onUninit();
	delete ERComponentGlobal::info;
}
#endif

#endif
