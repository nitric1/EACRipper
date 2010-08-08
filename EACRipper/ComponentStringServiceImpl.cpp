#include "Defaults.h"

#include "ComponentServiceImpl.h"

#include <boost/detail/endian.hpp>

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
			: charset("UTF-8"), cv(nullptr)
		{
			makeConverter();
		}

		StringCharsetConverter::~StringCharsetConverter()
		{
			if(cv != nullptr)
				ucnv_close(cv);
		}

		void StringCharsetConverter::makeConverter()
		{
			if(cv != nullptr)
				ucnv_close(cv);

			UErrorCode err;
			cv = ucnv_open(charset.c_str(), &err);

			if(U_FAILURE(err) && charset != "iso-8859-1")
			{
				string basecs = charset;
				charset = "iso-8859-1";
				try
				{
					makeConverter();
				}
				catch(int)
				{
					throw(runtime_error("Specified charset " + basecs + " cannot be opened and so is iso-8859-1."));
				}
			}
			else if(U_FAILURE(err))
				throw(0);

			ucnv_setSubstString(cv, L"\x3F", -1, &err); // "?" U+003F QUESTION MARK
		}

		const char *StringCharsetConverter::getCharset() const
		{
			return charset.c_str();
		}

		bool StringCharsetConverter::setCharset(const char *icharset)
		{
			charset = icharset;
			makeConverter();
			return true;
		}

		// TODO: Implement StringCharsetConverter's functions.
		size_t StringCharsetConverter::getConvertedLengthToUTF16(const char *str, size_t length)
		{
			UErrorCode err;
			int32_t size = ucnv_toUChars(cv, nullptr, 0, str, static_cast<int32_t>(length), &err);
			if(U_FAILURE(err))
				return static_cast<size_t>(-1);
			if(length == numeric_limits<size_t>::max())
				++ size;
			return static_cast<size_t>(size);
		}

		size_t StringCharsetConverter::getConvertedLengthFromUTF16(const wchar_t *str, size_t length)
		{
			UErrorCode err;
			int32_t size = ucnv_fromUChars(cv, nullptr, 0, str, static_cast<int32_t>(length), &err);
			if(U_FAILURE(err))
				return static_cast<size_t>(-1);
			if(length == numeric_limits<size_t>::max())
				++ size;
			return static_cast<size_t>(size);
		}

		size_t StringCharsetConverter::convertToUTF16(wchar_t *toString, size_t toBufferLength, const char *fromString, size_t fromLength)
		{
			UErrorCode err;
			int32_t size = ucnv_toUChars(cv, toString, static_cast<int32_t>(toBufferLength), fromString, static_cast<int32_t>(fromLength), &err);
			if(U_FAILURE(err))
				return static_cast<size_t>(-1);
			if(fromLength == numeric_limits<size_t>::max())
				++ size;
			return static_cast<size_t>(size);
		}

		size_t StringCharsetConverter::convertFromUTF16(char *toString, size_t toBufferLength, const wchar_t *fromString, size_t fromLength)
		{
			UErrorCode err;
			int32_t size = ucnv_fromUChars(cv, toString, static_cast<int32_t>(toBufferLength), fromString, static_cast<int32_t>(fromLength), &err);
			if(U_FAILURE(err))
				return static_cast<size_t>(-1);
			if(fromLength == numeric_limits<size_t>::max())
				++ size;
			return static_cast<size_t>(size);
		}

		CharsetDetector::~CharsetDetector()
		{
		}

		IERServiceStringConverter *CharsetDetector::detect(const char *str)
		{
			// TODO: Implement this function.
			// return ServicePointerManager::instance().append<IERServiceStringConverter>(c);
			return nullptr;
		}
	}
}
