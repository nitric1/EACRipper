#pragma once

#include "ERUUID.h"

#include "IERAllocator.h"
#include "IERFile.h"

typedef struct _ExtractorInformation
{
	const wchar_t *name; // e.g. "RAR Archive"
	const wchar_t *extension; // e.g. L"rar"
	const wchar_t *mime; // e.g. L"application/x-rar-compressed;application/x-rar;application/rar"
	size_t magicLength;
	const uint8_t *magic;
} ExtractorInformation;

class IERComponentArchiveExtractor
{
public:
	/**
	 * Destructor for safe deriving.
	 */
	virtual ~IERComponentArchiveExtractor() = 0 {}

public:
	/**
	 * Return the information of the extractor.
	 * @return ExtractorInformation which includes name, extension, some other information.
	 */
	virtual const ExtractorInformation &getInfo() const = 0;

	/**
	 * Set a stream reader of the archive.
	 * @param stream A pointer to a stream reader of the archive.
	 * @return true if a reader is set correctly, false otherwise.
	 */
	virtual bool setStream(IERStreamReader *stream) = 0;

	/**
	 * Clean up the extractor to let the extractor not use a reader and some reading information moreover.
	 * @return true if closed correctly, false otherwise.
	 */
	virtual bool close() = 0;

	/**
	 * Return file count of the archive.
	 * @return File count.
	 */
	virtual uint32_t getFileCount() const = 0;

	/**
	 * Return total size of files in the archive.
	 * @return Total size in bytes.
	 */
	virtual uint64_t getTotalSize() const = 0;

	/**
	 * Extract the archive using root.
	 * @param root A pointer to the directory where the archive is extracted.
	 * @return Total size of extracted data in byte if succeeded, std::numeric_limits<uint64_t>::max() in <limits> if failed.
	 */
	virtual uint64_t extract(IERDirectory *root) = 0;
};
