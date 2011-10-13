#pragma once

#include "ERUUID.h"

#include <limits>

// Cannot request this directly. Request IERServiceStringCodepageConverter or IERServiceStringCharsetConverter instead.

/// A basic interface of string converter.
/// @author Wondong LEE
class IERServiceStringConverter
{
public:
	/**
	 * Destructor for safe destruction.
	 */
	virtual ~IERServiceStringConverter() = 0 {}

public:
	/// Gets a converted string(wchar_t array)'s length. The given string's encoding is local encoding, and the converted string's encoding is UTF-16.
	/// For example, <code>getConvertedLengthToUTF16("ABCD", 4)</code> is 4, and <code>getConvertedLengthToUTF16("\xEA\xB0\x80\xEA\xB0\x81")</code> from UTF-8 (= <code>L"\uAC00\uAC01"</code>) is 3 (null character included).
	/// @author Wondong LEE
	/// @param fromString The string to convert.
	/// @param length	  The length of <code>fromString</code>. If you want to convert entire string (including null character), let this be {@code std::numeric_limits<size_t>::max()}.
	/// @return The converted length.
	virtual size_t getConvertedLengthToUTF16(const char *fromString, size_t length = std::numeric_limits<size_t>::max()) = 0;

	/// Gets a converted string(char array)'s length. The given string's encoding is UTF-16, and the converted string's encoding is local encoding.
	/// For example, <code>getConvertedLengthFromUTF16(L"ABCD", 4)</code> is 4, and <code>getConvertedLengthFromUTF16(L"\uAC00\uAC01")</code> to UTF-8 (= <code>"\xEA\xB0\x80\xEA\xB0\x81"</code>) is 7 (null character included).
	/// @author Wondong LEE
	/// @param fromString The string to convert.
	/// @param length	  The length of <code>fromString</code>. If you want to convert entire string (including null character), let this be {@code std::numeric_limits<size_t>::max()}.
	/// @return The converted length.
	virtual size_t getConvertedLengthFromUTF16(const wchar_t *fromString, size_t length = std::numeric_limits<size_t>::max()) = 0;

	/// Converts a given string from local encoding to UTF-16.
	/// The buffer's length must be equal to or be greater than <code>getConvertedLengthToUTF16(fromString, fromLength)</code>.
	/// @author Wondong LEE
	/// @param [out] toString The converted string buffer.
	/// @param toBufferLength The length of the buffer.
	/// @param fromString	  The string to convert.
	/// @param fromLength	  The length of <code>fromString</code>.
	/// @return The converted length.
	/// @see #getConvertedLengthToUTF16
	virtual size_t convertToUTF16(wchar_t *toString, size_t toBufferLength, const char *fromString, size_t fromLength = std::numeric_limits<size_t>::max()) = 0;

	/// Converts a given string from UTF-16 to local encoding.
	/// The buffer's length must be equal to or be greater than <code>getConvertedLengthFromUTF16(fromString, fromLength)</code>.
	/// @author Wondong LEE
	/// @param [out] toString The converted string buffer.
	/// @param toBufferLength The length of the buffer.
	/// @param fromString	  The string to convert.
	/// @param fromLength	  The length of <code>fromString</code>.
	/// @return The converted length.
	/// @see #getConvertedLengthFromUTF16
	virtual size_t convertFromUTF16(char *toString, size_t toBufferLength, const wchar_t *fromString, size_t fromLength = std::numeric_limits<size_t>::max()) = 0;
};

/// A string converter class using Windows API.
/// @author Wondong LEE
class IERServiceStringCodepageConverter : public IERServiceStringConverter
{
public:
	/**
	 * Destructor for safe destruction.
	 */
	virtual ~IERServiceStringCodepageConverter() = 0 {}

public:
	/// Gets the current codepage.
	/// @author Wondong LEE
	/// @return The current codepage.
	virtual uint32_t getCodepage() const = 0;

	/// Sets a new codepage.
	/// @author Wondong LEE
	/// @param codepage The new codepage.
	/// @return true if it succeeds.
	virtual bool setCodepage(uint32_t codepage) = 0;
};

/// A string converter class using ICU library.
/// @author Wondong LEE
class IERServiceStringCharsetConverter : public IERServiceStringConverter
{
public:
	/**
	 * Destructor for safe destruction.
	 */
	virtual ~IERServiceStringCharsetConverter() = 0 {}

public:
	/// Gets the current character set.
	/// @author Wondong LEE
	/// @return nullptr if it fails, else the character set.
	virtual const char *getCharset() const = 0;

	/// Sets a new character set.
	/// @author Wondong LEE
	/// @param charset The new charset.
	/// @return true if it succeeds.
	virtual bool setCharset(const char *charset) = 0;
};

/// A string's character set detector class.
/// @author Wondong LEE
class IERServiceCharsetDetector
{
public:
	/**
	 * Destructor for safe destruction.
	 */
	virtual ~IERServiceCharsetDetector() = 0 {}

public:
	/// Detects a given string's character set.
	/// @author Wondong LEE
	/// @param str The string to detect.
	/// @return nullptr if it fails, else proper <code>IERServiceStringConverter</code> class to convert the string.
	/// @see IERServiceStringConverter
	virtual IERServiceStringConverter *detect(const char *str) = 0;
};

template<>
const ERUUID ERServiceUUID<IERServiceStringCodepageConverter>::uuid
	= ERUUID(0x2CE9BCFA, 0xD4B0, 0x4387, 0x8B41, 0x19, 0xB3, 0x53, 0xDB, 0x1C, 0x48);
// 2CE9BCFA-D4B0-4387-8B41-19B353DB1C48

template<>
const ERUUID ERServiceUUID<IERServiceStringCharsetConverter>::uuid
	= ERUUID(0x2EE46411, 0xFB7E, 0x493F, 0xBD34, 0xF6, 0x9D, 0xA7, 0x73, 0xED, 0x78);
// 2EE46411-FB7E-493F-BD34-F69DA773ED78

template<>
const ERUUID ERServiceUUID<IERServiceCharsetDetector>::uuid
	= ERUUID(0x6C1B8B02, 0xE32B, 0x4725, 0xAFAE, 0xD7, 0xF8, 0x88, 0x15, 0x98, 0x42);
// 6C1B8B02-E32B-4725-AFAE-D7F888159842
