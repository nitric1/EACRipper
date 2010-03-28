#pragma once

#ifndef _INCLUDE_ERCOMPONENTFACTORY_
#define _INCLUDE_ERCOMPONENTFACTORY_

namespace ERComponent
{
	template<typename T>
	class ServiceFactory
	{
	private:
		T *ptr;

	public:
		ServiceFactory() { ptr = ApplicationInfo::instance().getApp().getServicePointer<T>(); }
		explicit ServiceFactory(const void *param) { ptr = ApplicationInfo::instance().getApp().getServicePointer<T>(param); }
		virtual ~ServiceFactory() {}

	public:
		T *getPtr() { return ptr; }
		const T *getPtr() const { return ptr; }

	public:
		T *operator ->() { return ptr; }
		const T *operator ->() const { return ptr; }
	};
}

#endif
