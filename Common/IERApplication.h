#pragma once

#include "ERUUID.h"
#include "IERComponentInfo.h"

#ifndef _INC_WINDOWS
#error Include <windows.h> before include this header!
#endif

class IERApplication
{
public:
	virtual ~IERApplication() = 0 {}

private:
	virtual void *getServicePointerImpl(const ERUUID &uuid, const void *param) = 0;
	virtual void removeServicePointerImpl(const ERUUID &uuid, void *ptr) = 0;

public:
	virtual HWND getWindow() const = 0;
	virtual void setInfo(IERComponentInfo *) = 0;

	template<typename T>
	T *getServicePointer() { return static_cast<T *>(getServicePointerImpl(ERServiceUUID<T>(), nullptr)); }
	template<typename T>
	T *getServicePointer(const void *param) { return static_cast<T *>(getServicePointerImpl(ERServiceUUID<T>(), param)); }
	template<typename T>
	void removeServicePointer(T *ptr) { removeServicePointerImpl(ERServiceUUID<T>(), ptr); }
};
