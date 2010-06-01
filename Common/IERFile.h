#pragma once

#include "ERUUID.h"

#include "IERStream.h"

class IERFile
{
public:
	/**
	 * Destructor for safe deriving.
	 */
	virtual ~IERFile() = 0 {}

public:
	virtual bool exists() = 0;
	virtual bool canRead() = 0;
	virtual bool canWrite() = 0;

	virtual IERStreamReader *getStreamReader(bool make = false) = 0;
	virtual IERStreamWriter *getStreamWriter(bool truncate = true) = 0;
};

class IERLocalFile : public IERFile
{
public:
	/**
	 * Destructor for safe deriving.
	 */
	virtual ~IERLocalFile() = 0 {}

public:
	virtual bool open(const wchar_t *path) = 0;
	virtual bool close() = 0;
};

class IERDirectory
{
public:
	/**
	 * Destructor for safe deriving.
	 */
	virtual ~IERDirectory() = 0 {}

public:
	virtual IERFile *getFile(const wchar_t *relativePath) = 0;
};

class IERLocalDirectory : public IERDirectory
{
public:
	/**
	 * Destructor for safe deriving.
	 */
	virtual ~IERLocalDirectory() = 0 {}

public:
	virtual bool open(const wchar_t *path, bool make = false) = 0;
	virtual bool close() = 0;
};

template<>
const ERUUID ERServiceUUID<IERLocalFile>::uuid
	= ERUUID(0x0C2AD544, 0xB29B, 0x4744, 0x9286, 0xA4, 0x82, 0x26, 0x86, 0x98, 0x5E);
// 0C2AD544-B29B-4744-9286-A4822686985E

template<>
const ERUUID ERServiceUUID<IERLocalDirectory>::uuid
	= ERUUID(0x5C947F5C, 0xB8D1, 0x4FA1, 0xAB01, 0x6D, 0xB0, 0xA9, 0xBF, 0xBB, 0x66);
// 5C947F5C-B8D1-4FA1-AB01-6DB0A9BFBB66
