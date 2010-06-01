#include "Defaults.h"

#include "ComponentServiceImpl.h"

using namespace std;

namespace EACRipper
{
	namespace ServiceImpl
	{
		StringCodepageConverter::StringCodepageConverter()
			: codepage(CP_ACP)
		{
		}

		StringCodepageConverter::~StringCodepageConverter()
		{
		}

		uint32_t StringCodepageConverter::getCodepage() const
		{
			return codepage;
		}

		bool StringCodepageConverter::setCodepage(uint32_t icodepage)
		{
			codepage = icodepage;
			return true;
		}

		size_t StringCodepageConverter::getConvertedLengthToUTF16(const char *str, size_t length)
		{
			return MultiByteToWideChar(codepage, 0, str, static_cast<int>(length), nullptr, 0);
		}

		size_t StringCodepageConverter::getConvertedLengthFromUTF16(const wchar_t *str, size_t length)
		{
			return WideCharToMultiByte(codepage, 0, str, static_cast<int>(length), nullptr, 0, nullptr, nullptr);
		}

		size_t StringCodepageConverter::convertToUTF16(wchar_t *toString, size_t toBufferLength, const char *fromString, size_t fromLength)
		{
			return MultiByteToWideChar(codepage, 0, fromString, static_cast<int>(fromLength), toString, static_cast<int>(toBufferLength));
		}

		size_t StringCodepageConverter::convertFromUTF16(char *toString, size_t toBufferLength, const wchar_t *fromString, size_t fromLength)
		{
			return WideCharToMultiByte(codepage, 0, fromString, static_cast<int>(fromLength), toString, static_cast<int>(toBufferLength), nullptr, nullptr);
		}

		StringCharsetConverter::StringCharsetConverter()
			: charset("UTF-8")
		{
		}

		StringCharsetConverter::~StringCharsetConverter()
		{
		}

		const char *StringCharsetConverter::getCharset() const
		{
			return charset.c_str();
		}

		bool StringCharsetConverter::setCharset(const char *icharset)
		{
			charset = icharset;
			return true;
		}

		// TODO: Implement StringCharsetConverter's functions.
		size_t StringCharsetConverter::getConvertedLengthToUTF16(const char *str, size_t length)
		{
			return 0;
		}

		size_t StringCharsetConverter::getConvertedLengthFromUTF16(const wchar_t *str, size_t length)
		{
			return 0;
		}

		size_t StringCharsetConverter::convertToUTF16(wchar_t *toString, size_t toBufferLength, const char *fromString, size_t fromLength)
		{
			return 0;
		}

		size_t StringCharsetConverter::convertFromUTF16(char *toString, size_t toBufferLength, const wchar_t *fromString, size_t fromLength)
		{
			return 0;
		}
	}
}
