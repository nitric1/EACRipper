#pragma once

class IERApplication;

#include "../Common/ERServiceGUID.h"

class IERApplication
{
public:
	virtual ~IERApplication() = 0;

private:
	virtual void *getServicePointerImpl(const ERServiceGUID &guid) = 0;

public:
	virtual HWND getWindow() const = 0;

	virtual bool registerMusicDecoder(const wchar_t *formatName, const wchar_t *ext) = 0;
	virtual bool registerMusicEncoder(const wchar_t *formatName, const wchar_t *ext) = 0;

	virtual bool registerArchiveExtractor(const wchar_t *formatName, const wchar_t *ext) = 0;

	template<typename T>
	T *getServicePointer() { return static_cast<T *>(getServicePointerImpl(ERServiceGUID<T>())); }
};
