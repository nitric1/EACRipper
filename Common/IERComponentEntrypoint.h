#pragma once

class IERComponent;

class IERComponent
{
public:
	virtual ~IERComponent() = 0;

public:
	virtual bool onInit() = 0;
	virtual bool onUninit() = 0;
};
