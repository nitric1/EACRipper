#include "Defaults.h"

#include "ComponentServiceImpl.h"
#include "Configure.h"

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

		vector<wstring> StringCharsetConverter::makeCharsetList()
		{
			vector<wstring> list;
			uint16_t n = ucnv_countAvailable();
			UErrorCode err = U_ZERO_ERROR;
			const char *name;
			wchar_t buf[48];
			StringCharsetConverter cv;
			for(uint16_t i = 0; i < n; ++ i)
			{
				name = ucnv_getAvailableName(i);
				name = ucnv_getStandardName(name, "MIME", &err);
				if(U_FAILURE(err) || name == nullptr)
				{
					err = U_ZERO_ERROR;
					continue;
				}
				if(cv.convertToUTF16(buf, 48, name) != static_cast<size_t>(-1))
					list.push_back(buf);
			}

			return move(list);
		}

		const vector<wstring> &StringCharsetConverter::getCharsetList()
		{
			static vector<wstring> list(makeCharsetList());
			return list;
		}

		bool StringCharsetConverter::makeConverter()
		{
			if(cv != nullptr)
				ucnv_close(cv);

			UErrorCode err = U_ZERO_ERROR;
			cv = ucnv_open(charset.c_str(), &err);

			if(U_FAILURE(err))
			{
				cv = nullptr;
				return false;
			}

			ucnv_setSubstString(cv, L"\x3F", -1, &err); // "?" U+003F QUESTION MARK
			return true;
		}

		const char *StringCharsetConverter::getCharset() const
		{
			return charset.c_str();
		}

		bool StringCharsetConverter::setCharset(const char *icharset)
		{
			if(charset == icharset)
				return true;
			charset = icharset;
			return makeConverter();
		}

		size_t StringCharsetConverter::getConvertedLengthToUTF16(const char *str, size_t length)
		{
			UErrorCode err = U_ZERO_ERROR;
			int32_t size = ucnv_toUChars(cv, nullptr, 0, str, static_cast<int32_t>(length), &err);
			if(err != U_ZERO_ERROR && err != U_BUFFER_OVERFLOW_ERROR && err != U_STRING_NOT_TERMINATED_WARNING)
				return static_cast<size_t>(-1);
			if(length == numeric_limits<size_t>::max())
				++ size;
			return static_cast<size_t>(size);
		}

		size_t StringCharsetConverter::getConvertedLengthFromUTF16(const wchar_t *str, size_t length)
		{
			UErrorCode err = U_ZERO_ERROR;
			int32_t size = ucnv_fromUChars(cv, nullptr, 0, str, static_cast<int32_t>(length), &err);
			if(err != U_ZERO_ERROR && err != U_BUFFER_OVERFLOW_ERROR && err != U_STRING_NOT_TERMINATED_WARNING)
				return static_cast<size_t>(-1);
			if(length == numeric_limits<size_t>::max())
				++ size;
			return static_cast<size_t>(size);
		}

		size_t StringCharsetConverter::convertToUTF16(wchar_t *toString, size_t toBufferLength, const char *fromString, size_t fromLength)
		{
			UErrorCode err = U_ZERO_ERROR;
			int32_t size = ucnv_toUChars(cv, toString, static_cast<int32_t>(toBufferLength), fromString, static_cast<int32_t>(fromLength), &err);
			if(err != U_ZERO_ERROR && err != U_BUFFER_OVERFLOW_ERROR && err != U_STRING_NOT_TERMINATED_WARNING)
				return static_cast<size_t>(-1);
			if(fromLength == numeric_limits<size_t>::max())
				++ size;
			if(size > 0 && toString[0] == L'\uFEFF') // remove byte order mark
				memmove(toString, toString + 1, (-- size) * sizeof(wchar_t));
			return static_cast<size_t>(size);
		}

		size_t StringCharsetConverter::convertFromUTF16(char *toString, size_t toBufferLength, const wchar_t *fromString, size_t fromLength)
		{
			UErrorCode err = U_ZERO_ERROR;
			int32_t size = ucnv_fromUChars(cv, toString, static_cast<int32_t>(toBufferLength), fromString, static_cast<int32_t>(fromLength), &err);
			if(err != U_ZERO_ERROR && err != U_BUFFER_OVERFLOW_ERROR && err != U_STRING_NOT_TERMINATED_WARNING)
				return static_cast<size_t>(-1);
			if(fromLength == numeric_limits<size_t>::max())
				++ size;
			return static_cast<size_t>(size);
		}

		CharsetDetector::CharsetDetector()
			: cd(nullptr)
		{
			UErrorCode err = U_ZERO_ERROR;
			cd = ucsdet_open(&err);
		}

		CharsetDetector::~CharsetDetector()
		{
			if(cd != nullptr)
				ucsdet_close(cd);
		}

		IERServiceStringConverter *CharsetDetector::detect(const char *str)
		{
			const char *name;
			if((str[0] != '\0' && str[0] == '\xFF') && (str[1] != '\0' && str[1] == '\xFE'))
				name = "UTF-16LE";
			else if((str[0] != '\0' && str[0] == '\xFE') && (str[1] != '\0' && str[1] == '\xFF'))
				name = "UTF-16BE";
			else
			{
				UErrorCode err = U_ZERO_ERROR;
				ucsdet_setText(cd, str, -1, &err);
				if(U_FAILURE(err))
					return nullptr;
				const UCharsetMatch *cm = ucsdet_detect(cd, &err);
				if(U_FAILURE(err))
					return nullptr;
				name = ucsdet_getName(cm, &err);
				if(U_FAILURE(err))
					return nullptr;
			}

			StringCharsetConverter *c = new StringCharsetConverter();
			if(!c->setCharset(name))
			{
				delete c;
				return nullptr;
			}

			return ServicePointerManager::instance().append<IERServiceStringConverter>(c);
		}
	}
}
