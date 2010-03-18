#pragma once

class IERComponentEntrypoint
{
public:
	virtual ~IERComponentEntrypoint() = 0 {}

public:
	virtual bool onInit() = 0;
	virtual bool onUninit() = 0;
};
