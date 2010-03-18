#pragma once

#include "ERUUID.h"

#ifndef _INC_WINDOWS
#error Include <windows.h> before include this header!
#endif

class IERApplication
{
public:
	virtual ~IERApplication() = 0 {}

private:
	virtual void *getServicePointerImpl(const ERUUID &uuid) = 0;

public:
	virtual HWND getWindow() const = 0;

	virtual bool registerMusicDecoder(const wchar_t *formatName, const wchar_t *ext) = 0;
	virtual bool registerMusicEncoder(const wchar_t *formatName, const wchar_t *ext) = 0;

	virtual bool registerArchiveExtractor(const wchar_t *formatName, const wchar_t *ext) = 0;

	template<typename T>
	T *getServicePointer() { return static_cast<T *>(getServicePointerImpl(ERServiceUUID<T>())); }
};
