#pragma once

class IERComponent;

#include "../Common/ERComponentInfo.h"
#include "../Common/IERApplication.h"

#include <cinttypes>

class IERComponent
{
public:
	virtual ~IERComponent() = 0;

public:
	virtual bool onInit() = 0;
	virtual bool onUninit() = 0;
};

#define DECLARE_COMPONENT(name, cls)
