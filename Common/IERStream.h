#pragma once

#include "ERUUID.h"

#include <cinttypes>

enum ERStreamSeekMode
{
	BEGIN,
	CURRENT,
	END
};

class IERStreamReader
{
public:
	/**
	 * Destructor for safe deriving.
	 */
	virtual ~IERStreamReader() = 0 {}

public:
	/**
	 * Return whether the file is usable or not.
	 * @return true if the file is usable, false otherwise.
	 **/
	virtual bool usable() const = 0;

	/**
	 * Get the file size.
	 * @return The file size in byte.
	 **/
	virtual uint64_t size() const = 0;

	/**
	 * Set the file pointer's position.
	 * @param pos Position to seek in byte.
	 * @param mode Seeking mode one of ERStreamSeekMode.
	 * @return true if the file is sought correctly, false otherwise.
	 */
	virtual bool seek(int64_t pos, int32_t mode) = 0;

	/**
	 * Get the file pointer's position from begin.
	 * @return The position of the file pointer in byte if succeeded, std::numeric_limits<uint64_t>::max() in <limits> if failed.
	 */
	virtual uint64_t tell() const = 0;

	/**
	 * Read the data from the file pointer's position.
	 * @param buffer Buffer in which data is to be stored.
	 * @param bufferSize The buffer's size in byte.
	 * @return Size of read data in byte if the data is read correctly, std::numeric_limits<size_t>::max() in <limits> if failed.
	 */
	virtual size_t read(void *buffer, size_t bufferSize) = 0;
};

class IERStreamWriter
{
public:
	/**
	 * Destructor for safe deriving.
	 */
	virtual ~IERStreamWriter() = 0 {}

public:
	/**
	 * Return whether the file is usable or not.
	 * @return true if the file is usable, false otherwise.
	 **/
	virtual bool usable() const = 0;

	/**
	 * Get the file size.
	 * @return The file size in byte.
	 **/
	virtual uint64_t size() const = 0;

	/**
	 * Set the file pointer's position.
	 * @param pos Position to seek in byte.
	 * @param mode Seeking mode one of ERStreamSeekMode.
	 * @return true if the file is sought correctly, false otherwise.
	 */
	virtual bool seek(int64_t pos, int32_t mode) = 0;

	/**
	 * Get the file pointer's position from begin.
	 * @return The position of the file pointer in byte.
	 */
	virtual uint64_t tell() const = 0;

	/**
	 * Write the data from the file pointer's position.
	 * @param buffer Buffer of data to be saved.
	 * @param bufferSize The buffer's size in byte.
	 * @return Size of written data in byte if the data is saved correctly, std::numeric_limits<size_t>::max() in <limits> if failed.
	 */
	virtual size_t write(const void *buffer, size_t bufferSize) = 0;
};

class IERFileStreamReader : public IERStreamReader
{
public:
	/**
	 * Destructor for safe deriving.
	 */
	virtual ~IERFileStreamReader() = 0 {}

public:
	virtual bool open(const wchar_t *path, bool make = false) = 0;
	virtual bool close() = 0;
};

class IERFileStreamWriter : public IERStreamWriter
{
public:
	/**
	 * Destructor for safe deriving.
	 */
	virtual ~IERFileStreamWriter() = 0 {}

public:
	virtual bool open(const wchar_t *path, bool truncate = true) = 0;
	virtual bool close() = 0;
};

class IERMemoryStreamReader : public IERStreamReader
{
public:
	/**
	 * Destructor for safe deriving.
	 */
	virtual ~IERMemoryStreamReader() = 0 {}

public:
	virtual bool open(const void *memory, size_t totalSize) = 0;
	virtual bool close() = 0;
};

template<>
const ERUUID ERServiceUUID<IERFileStreamReader>::uuid
	= ERUUID(0x14CFC829, 0x7B48, 0x4786, 0xAFB2, 0x01, 0xA2, 0x4E, 0x39, 0xB7, 0x16);
// 14CFC829-7B48-4786-AFB2-01A24E39B716

template<>
const ERUUID ERServiceUUID<IERFileStreamWriter>::uuid
	= ERUUID(0x7609A084, 0xABE8, 0x462B, 0x96AB, 0x5F, 0x61, 0x73, 0x03, 0xA6, 0x93);
// 7609A084-ABE8-462B-96AB-5F617303A693

template<>
const ERUUID ERServiceUUID<IERMemoryStreamReader>::uuid
	= ERUUID(0x34AA38C4, 0x9551, 0x44B9, 0x8DA8, 0x26, 0x14, 0x62, 0x93, 0xD5, 0x71);
// 34AA38C4-9551-44B9-8DA8-26146293D571
