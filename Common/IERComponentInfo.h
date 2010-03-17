#pragma once

class IERComponentInfo
{
public:
	virtual const wchar_t *getSDKVersion() = 0;
	virtual const wchar_t *getComponentName() = 0;
	virtual const wchar_t *getComponentVersion() = 0;
};
