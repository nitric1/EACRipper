#pragma once

#define EACRIPPER_COMPONENT_SDK_VERSION "2.0.0"

class ERApplicationInfo;
class ERComponentInfo;

template<typename T>
class ERServiceFactory;

#include "../Common/IERApplication.h"
#include "../Common/IERComponentEntrypoint.h"
#include "../Common/IERComponentInfo.h"
#include "../Common/IERStreamReader.h"
#include "../Common/IERStreamWriter.h"
#include "../Common/ERServiceGUID.h"

#include <cinttypes>

class ERApplicationInfo
{
private:
	IERApplication *theApp;

private:
	ERApplicationInfo() : theApp(NULL) {}

private:
	void setApp(IERApplication *app) { theApp = app; }

public:
	static ERApplicationInfo &instance() { ERApplicationInfo theInstance; return theInstance; }
	IERApplication &getApp() { return theApp; }

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

	extern const IERComponentEntrypoint *componentEntry;
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
};

#define DECLARE_COMPONENT(name, version) \
	namespace ComponentGlobal { \
		const wchar_t *componentName = name; \
		const wchar_t *componentVersion = version; \
		const IERComponentEntrypoint *componentEntry = NULL; \
	}

#if defined(_WINDOWS) && defined(_USRDLL)
#include <windows.h>

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
