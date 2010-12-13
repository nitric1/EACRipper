#include "Defaults.h"

#include "MemoryStream.h"

namespace EACRipper
{
	MemoryStreamReader::MemoryStreamReader()
		: memory(nullptr), totalSize(0), pos(0)
	{
	}

	MemoryStreamReader::MemoryStreamReader(const void *imemory, size_t itotalSize)
		: memory(nullptr), totalSize(0), pos(0)
	{
		open(imemory, itotalSize);
	}

	MemoryStreamReader::~MemoryStreamReader()
	{
		close();
	}

	bool MemoryStreamReader::open(const void *imemory, size_t itotalSize)
	{
		memory = imemory;
		totalSize = itotalSize;
		pos = 0;
		return true;
	}

	bool MemoryStreamReader::close()
	{
		memory = nullptr;
		totalSize = 0;
		pos = 0;
		return true;
	}

	bool MemoryStreamReader::usable() const
	{
		return memory != nullptr;
	}

	uint64_t MemoryStreamReader::size() const
	{
		return totalSize;
	}

	bool MemoryStreamReader::seek(int64_t ipos, int32_t mode)
	{
		switch(mode)
		{
		case BEGIN:
			if(ipos > static_cast<int64_t>(totalSize))
				ipos = static_cast<int64_t>(totalSize);
			else if(ipos < 0)
				ipos = 0;
			pos = static_cast<size_t>(ipos);
			break;

		case CURRENT:
			if(ipos < 0 && -ipos > static_cast<int64_t>(pos))
				pos = 0;
			else
			{
				pos = static_cast<size_t>(static_cast<int64_t>(pos) + ipos);
				if(pos > totalSize)
					pos = totalSize;
			}
			break;

		case END:
			if(ipos > static_cast<int64_t>(totalSize))
				ipos = static_cast<int64_t>(totalSize);
			else if(ipos < 0)
				ipos = 0;
			pos = static_cast<size_t>(static_cast<int64_t>(totalSize) - ipos);
			break;
		}

		return true;
	}

	uint64_t MemoryStreamReader::tell() const
	{
		return pos;
	}

	size_t MemoryStreamReader::read(void *buffer, size_t bufferSize)
	{
		bufferSize = min(bufferSize, totalSize - pos);
		memcpy(buffer, static_cast<const uint8_t *>(memory) + pos, bufferSize);
		pos += bufferSize;
		return bufferSize;
	}
}
