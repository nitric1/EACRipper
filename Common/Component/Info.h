#pragma once

#ifndef _INCLUDE_ERCOMPONENTINFO_
#define _INCLUDE_ERCOMPONENTINFO_

#include "IERComponentEntrypoint.h"

namespace ERComponentGlobal
{
	extern const wchar_t *componentName;
	extern const wchar_t *componentVersion;

	extern IERComponentEntrypoint *componentEntry;

	extern IERComponentInfo *info;
}

namespace ERComponent
{
	class ApplicationInfo
	{
	private:
		IERApplication *theApp;

	private:
		ApplicationInfo() : theApp(NULL) {}

	private:
		void setApp(IERApplication *app) { theApp = app; }

	public:
		static ApplicationInfo &instance() { static ApplicationInfo theInstance; return theInstance; }
		IERApplication &getApp() { return *theApp; }

		friend void ::initComponent(IERApplication *);
	};

	template<typename T>
	class EntrypointImpl : public IERComponentEntrypoint
	{
	private:
		T t;

	public:
		virtual bool onInit() { return t.onInit(); }
		virtual bool onUninit() { return t.onUninit(); }
	};

	template<typename T, typename Impl = EntrypointImpl<T> >
	class EntrypointRegister
	{
	public:
		EntrypointRegister()
		{
			ERComponentGlobal::componentEntry = new Impl;
		}

		~EntrypointRegister()
		{
			delete ERComponentGlobal::componentEntry;
		}
	};

	class Info : public IERComponentInfo
	{
	public:
		virtual const wchar_t *getSDKVersion() { return EACRIPPER_COMPONENT_SDK_VERSION; }
		virtual const wchar_t *getComponentName() { return ERComponentGlobal::componentName; }
		virtual const wchar_t *getComponentVersion() { return ERComponentGlobal::componentVersion; }
		virtual bool isDebug()
		{
#if defined(DEBUG) || defined(_DEBUG)
			return true;
#else
			return false;
#endif
		}
	};
}

#endif
