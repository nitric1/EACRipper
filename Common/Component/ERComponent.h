#pragma once

#define __STDC_FORMAT_MACROS
#include <cinttypes>

#include <windows.h>

#define EACRIPPER_COMPONENT_SDK_VERSION L"2.0.0"

class ERApplicationInfo;
class ERComponentInfo;

template<typename T>
class ERServiceFactory;

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

class ERApplicationInfo
{
private:
	IERApplication *theApp;

private:
	ERApplicationInfo() : theApp(NULL) {}

private:
	void setApp(IERApplication *app) { theApp = app; }

public:
	static ERApplicationInfo &instance() { static ERApplicationInfo theInstance; return theInstance; }
	IERApplication &getApp() { return *theApp; }

	friend void initComponent(IERApplication *);
};

template<typename T>
class ERServiceFactory
{
private:
	T *ptr;

public:
	ERServiceFactory() { ptr = ERApplicationInfo::instance().getApp().getServicePointer<T>(); }
	T *operator ->() { return ptr; }
};

namespace ComponentGlobal
{
	extern const wchar_t *componentName;
	extern const wchar_t *componentVersion;

	extern IERComponentEntrypoint *componentEntry;
}

template<typename T>
class ERComponentEntrypointFactory
{
public:
	ERComponentEntrypointFactory()
	{
		ComponentGlobal::componentEntry = new T;
	}

	~ERComponentEntrypointFactory()
	{
		delete ComponentGlobal::componentEntry;
	}
};

class ERComponentInfo : public IERComponentInfo
{
public:
	virtual const wchar_t *getSDKVersion() { return EACRIPPER_COMPONENT_SDK_VERSION; }
	virtual const wchar_t *getComponentName() { return ComponentGlobal::componentName; }
	virtual const wchar_t *getComponentVersion() { return ComponentGlobal::componentVersion; }
	virtual bool isDebug()
	{
#if defined(DEBUG) || defined(_DEBUG)
		return true;
#else
		return false;
#endif
	}
};

#define DECLARE_COMPONENT(name, version) \
	namespace ComponentGlobal { \
		const wchar_t *componentName = name; \
		const wchar_t *componentVersion = version; \
		IERComponentEntrypoint *componentEntry = NULL; \
	}

#if defined(_WINDOWS) && defined(_USRDLL)
extern "C" __declspec(dllexport) void initComponent(IERApplication *app)
{
	ERApplicationInfo::instance().setApp(app);
	if(ComponentGlobal::componentEntry != NULL)
		ComponentGlobal::componentEntry->onInit();
}

extern "C" __declspec(dllexport) void uninitComponent()
{
	if(ComponentGlobal::componentEntry != NULL)
		ComponentGlobal::componentEntry->onUninit();
}
#endif
