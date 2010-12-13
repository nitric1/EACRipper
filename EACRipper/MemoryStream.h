#pragma once

#include "IERStream.h"

namespace EACRipper
{
	class MemoryStreamReader : public IERMemoryStreamReader
	{
	private:
		const void *memory;
		size_t totalSize;
		size_t pos;

	public:
		MemoryStreamReader();
		MemoryStreamReader(const void *, size_t);
		virtual ~MemoryStreamReader();

	public:
		virtual bool open(const void *, size_t);
		virtual bool close();
		virtual bool usable() const;
		virtual uint64_t size() const;
		virtual bool seek(int64_t, int32_t);
		virtual uint64_t tell() const;
		virtual size_t read(void *, size_t);
	};
}
