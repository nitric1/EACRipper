#pragma once

class IERAllocator
{
public:
	virtual ~IERAllocator() = 0 {}

public:
	virtual void *alloc() = 0;
	virtual void free(void *) = 0;
};
