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

#if defined(_WINDOWS) && defined(_USRDLL)
#include <windows.h>

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
	static ERApplicationInfo &instance() { ERApplicationInfo theInstance; return theInstance; }
	IERApplication &getApp() { return theApp; }
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

template<typename T>
class ERComponentEntrypointFactory
{
};

#define DECLARE_COMPONENT(name, version, entry) \
	class ERComponentInfo : public IERComponentInfo \
	{ \
		virtual const wchar_t *getSDKVersion() { return EACRIPPER_COMPONENT_SDK_VERSION; } \
		virtual const wchar_t *getComponentName() { return name; } \
		virtual const wchar_t *getComponentVersion() { return version; } \
	}; \
	extern "C" __declspec(dllexport) void initComponent(IERApplication *) \
	{ \
		; \
	} \
	extern "C" __declspec(dllexport) void uninitComponent() \
	{ \
	}
