#pragma once

#include <cinttypes>

class IERStreamReader
{
public:
	typedef enum _SeekMode
	{
		BEGIN,
		CURRENT,
		END
	} SeekMode;

public:
	/**
	 * Destructor for safe deriving.
	 */
	virtual ~IERStreamReader() = 0 {}

	/**
	 * Get the file size.
	 * @return The file size in byte.
	 **/
	virtual uint64_t __stdcall size() const = 0;

	/**
	 * Set the file pointer's position.
	 * @param pos Position to seek in byte.
	 * @param mode Seeking mode.
	 * @return true if the file is sought correctly, false otherwise.
	 */
	virtual bool __stdcall seek(uint64_t pos, SeekMode mode) = 0;

	/**
	 * Get the file pointer's position from begin.
	 * @return The position of the file pointer in byte.
	 */
	virtual uint64_t __stdcall tell() const = 0;

	/**
	 * Read the data from the file pointer's position.
	 * @param buffer Buffer in which data is to be stored.
	 * @param bufferSize The buffer's size in byte.
	 * @return Size of read data in byte if succeeded, std::numeric_limits<uint64_t>::max() in <limits> if failed.
	 */
	virtual uint64_t __stdcall read(void *buffer, size_t bufferSize) = 0;
};
