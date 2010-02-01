#pragma once

#include <cinttypes>

class IERStreamWriter
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
	virtual ~IERStreamWriter() = 0 {}

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
	 * Write the data from the file pointer's position.
	 * @param buffer Buffer of data to be saved.
	 * @param bufferSize The buffer's size in byte.
	 * @return true if the data is saved correctly, false otherwise.
	 */
	virtual bool __stdcall write(const void *buffer, size_t bufferSize) = 0;
};
