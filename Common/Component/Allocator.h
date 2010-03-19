#pragma once

#ifndef _INCLUDE_ERCOMPONENTARCHIVE_
#define _INCLUDE_ERCOMPONENTARCHIVE_

#include "IERAllocator.h"

namespace ERComponent
{
	template<typename T>
	class Allocator : public IERAllocator
	{
	private:
		~Allocator() {}

	public:
		static Allocator<T> &instance() { static Allocator<T> inst; return inst; }

	public:
		virtual void *alloc() { return new T; }
		virtual void free(void *p) { delete static_cast<T *>(p); }
	};
}

#endif
