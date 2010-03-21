#pragma once

#include "ERUUID.h"

#include <limits>

// Cannot request this directly. Request IERServiceStringCodepageConverter or IERServiceStringCharsetConverter instead.
class IERServiceStringConverter
{
public:
	/**
	 * Destructor for save deriving.
	 */
	virtual ~IERServiceStringConverter() = 0 {}

public:
	virtual size_t getConvertedLength(const char *, size_t = std::numeric_limits<size_t>::max()) = 0;

	virtual size_t getConvertedLength(const wchar_t *, size_t = std::numeric_limits<size_t>::max()) = 0;

	virtual size_t convert(wchar_t *, size_t, const char *, size_t = std::numeric_limits<size_t>::max()) = 0;

	virtual size_t convert(char *, size_t, const wchar_t *, size_t = std::numeric_limits<size_t>::max()) = 0;
};

class IERServiceStringCodepageConverter : public IERServiceStringConverter
{
public:
	/**
	 * Destructor for save deriving.
	 */
	virtual ~IERServiceStringCodepageConverter() = 0 {}

public:
	virtual uint32_t getCodepage() const = 0;

	virtual bool setCodepage(uint32_t codepage);
};

class IERServiceStringCharsetConverter : public IERServiceStringConverter
{
public:
	/**
	 * Destructor for save deriving.
	 */
	virtual ~IERServiceStringCharsetConverter() = 0 {}

public:
	virtual const char *getCharset() const = 0;

	virtual bool setCodepage(const char *charset) = 0;
};

template<>
const ERUUID ERServiceUUID<IERServiceStringCodepageConverter>::uuid
	= ERUUID(0x2CE9BCFA, 0xD4B0, 0x4387, 0x8B41, 0x19, 0xB3, 0x53, 0xDB, 0x1C, 0x48);
// 2CE9BCFA-D4B0-4387-8B41-19B353DB1C48

template<>
const ERUUID ERServiceUUID<IERServiceStringCharsetConverter>::uuid
	= ERUUID(0x2EE46411, 0xFB7E, 0x493F, 0xBD34, 0xF6, 0x9D, 0xA7, 0x73, 0xED, 0x78);
// 2EE46411-FB7E-493F-BD34-F69DA773ED78
