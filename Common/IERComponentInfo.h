#pragma once

class IERComponentInfo
{
public:
	virtual const wchar_t *getSDKVersion() const = 0;
	virtual const wchar_t *getComponentName() const = 0;
	virtual const wchar_t *getComponentVersion() const = 0;
	virtual bool isDebug() const = 0;
};
