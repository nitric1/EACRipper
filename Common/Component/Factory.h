#pragma once

#ifndef _INCLUDE_ERCOMPONENTFACTORY_
#define _INCLUDE_ERCOMPONENTFACTORY_

namespace ERComponent
{
	template<typename T>
	class ERServiceFactory
	{
	private:
		T *ptr;

	public:
		ERServiceFactory() { ptr = ERApplicationInfo::instance().getApp().getServicePointer<T>(); }
		T *operator ->() { return ptr; }
	};
}

#endif
