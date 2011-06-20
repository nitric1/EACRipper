#pragma once

#include <cstdio>
#include <cstdlib>

#include <cinttypes>

#include <string>
#include <sstream>
#include <iosfwd>
#include <iomanip>
#include <algorithm>

#include "EREndian.h"

class ERUUID;

template<typename T>
class ERServiceUUID;

template<typename T, typename Traits>
std::basic_ostream<T, Traits> &operator <<(std::basic_ostream<T, Traits> &, const ERUUID &);

#pragma pack(push, 1)
union ERUUIDData
{
	struct
	{
		uint32_t val1;
		uint16_t val2;
		uint16_t val3;
		uint16_t val4;
		union
		{
			struct
			{
				uint8_t v1;
				uint8_t v2;
				uint8_t v3;
				uint8_t v4;
				uint8_t v5;
				uint8_t v6;
			} unpacked;
			uint8_t raw[6];
			struct
			{
				uint32_t v1;
				uint16_t v2;
			} packed;
		} val5;
	} unpacked;
	uint8_t raw[16];
	struct
	{
		uint64_t v1;
		uint64_t v2;
	} packed;
};
#pragma pack(pop)

class ERUUID
{
private:
	union ERUUIDData data;

public:
	static const size_t unpackedSize = 16;

public:
	ERUUID() : data() {}
	ERUUID(const ERUUID &uuid) : data(uuid.data) {}
	ERUUID(const ERUUIDData &idata) : data(idata) {}
	template<typename T>
	ERUUID(const ERServiceUUID<T> &preset) : data(preset.uuid.data) {}
	explicit ERUUID(const uint8_t *idata) : data()
	{
		data.packed.v1 = *reinterpret_cast<const uint64_t *>(idata);
		data.packed.v2 = *reinterpret_cast<const uint64_t *>(idata + 8);
	}
	explicit ERUUID(uint32_t ival1, uint16_t ival2, uint16_t ival3, uint16_t ival4,
		uint8_t ival5_1, uint8_t ival5_2, uint8_t ival5_3, uint8_t ival5_4, uint8_t ival5_5, uint8_t ival5_6)
		: data()
	{
		data.unpacked.val1 = ival1;
		data.unpacked.val2 = ival2;
		data.unpacked.val3 = ival3;
		data.unpacked.val4 = ival4;
		data.unpacked.val5.unpacked.v1 = ival5_1;
		data.unpacked.val5.unpacked.v2 = ival5_2;
		data.unpacked.val5.unpacked.v3 = ival5_3;
		data.unpacked.val5.unpacked.v4 = ival5_4;
		data.unpacked.val5.unpacked.v5 = ival5_5;
		data.unpacked.val5.unpacked.v6 = ival5_6;
	}
	explicit ERUUID(uint32_t ival1, uint16_t ival2, uint16_t ival3, uint16_t ival4, const uint8_t *ival5) : data()
	{
		data.unpacked.val1 = ival1;
		data.unpacked.val2 = ival2;
		data.unpacked.val3 = ival3;
		data.unpacked.val4 = ival4;
		data.unpacked.val5.packed.v1 = *reinterpret_cast<const uint32_t *>(ival5);
		data.unpacked.val5.packed.v2 = *reinterpret_cast<const uint16_t *>(ival5 + 4);
	}
	ERUUID(const char *str) : data() { parseString(str); }
	ERUUID(const wchar_t *str) : data() { parseString(str); }

public:
	ERUUID &operator =(const ERUUID &uuid)
	{
		data.packed = uuid.data.packed;
		return *this;
	}
	ERUUID &operator =(const ERUUIDData &idata)
	{
		data = idata;
		return *this;
	}
	template<typename T>
	ERUUID &operator =(const ERServiceUUID<T> &preset)
	{
		*this = preset.uuid;
		return *this;
	}
	ERUUID &operator =(const char *str)
	{
		parseString(str);
		return *this;
	}
	ERUUID &operator =(const wchar_t *str)
	{
		parseString(str);
		return *this;
	}

	bool operator ==(const ERUUID &uuid) const
	{
		return data.packed.v1 == uuid.data.packed.v1 && data.packed.v2 == uuid.data.packed.v2;
	}

	bool operator !=(const ERUUID &uuid) const
	{
		return !(*this == uuid);
	}

	operator const uint8_t *() const
	{
		return unpackedData();
	}

	operator union ERUUIDData() const
	{
		return getData();
	}

private:
	static uint8_t hexDigit(char ch)
	{
		if(ch >= '0' && ch <= '9')
			return static_cast<uint8_t>(ch - '0');
		else if(ch >= 'A' && ch <= 'F')
			return static_cast<uint8_t>(ch - 'A' + 10);
		else if(ch >= 'a' && ch <= 'f')
			return static_cast<uint8_t>(ch - 'a' + 10);
		return 0xFF;
	}

	static uint8_t hexDigit(wchar_t ch)
	{
		return hexDigit(static_cast<char>(ch));
	}

	bool parseString(const char *str)
	{
		bool next = false;
		uint8_t ndata;
		for(size_t i = 0; *str != '\0' && i < unpackedSize; ++ str)
		{
			if((*str >= '0' && *str <= '9') || (*str >= 'A' && *str <= 'F') || (*str >= 'a' && *str <= 'f'))
			{
				if(next)
				{
					data.raw[i ++] = (ndata << 4) | hexDigit(*str);
					next = false;
				}
				else
				{
					ndata = hexDigit(*str);
					next = true;
				}
			}
		}

		EREndian::B2N(data.unpacked.val1);
		EREndian::B2N(data.unpacked.val2);
		EREndian::B2N(data.unpacked.val3);
		EREndian::B2N(data.unpacked.val4);

		return true;
	}

	bool parseString(const wchar_t *str)
	{
		bool next = false;
		uint8_t ndata;
		for(size_t i = 0; *str != L'\0' && i < unpackedSize; ++ str)
		{
			if((*str >= L'0' && *str <= L'9') || (*str >= L'A' && *str <= L'F') || (*str >= L'a' && *str <= L'f'))
			{
				if(next)
				{
					data.raw[i ++] = (ndata << 4) | hexDigit(*str);
					next = false;
				}
				else
				{
					ndata = hexDigit(*str);
					next = true;
				}
			}
		}

		EREndian::B2N(data.unpacked.val1);
		EREndian::B2N(data.unpacked.val2);
		EREndian::B2N(data.unpacked.val3);
		EREndian::B2N(data.unpacked.val4);

		return true;
	}

public:
	void toString(char *str, size_t buflen) const
	{
		std::string s;
		toString(s);
		strcpy_s(str, buflen, s.c_str());
	}

	void toString(wchar_t *str, size_t buflen) const
	{
		std::wstring s;
		toString(s);
		wcscpy_s(str, buflen, s.c_str());
	}

	template<typename T, typename Traits, typename Alloc>
	void toString(std::basic_string<T, Traits, Alloc> &str) const
	{
		std::basic_ostringstream<T, Traits, Alloc> s;
		s << std::hex << std::uppercase << std::setfill(T('0'))
		  << std::setw(8) << data.unpacked.val1
		  << T('-') << std::setw(4) << data.unpacked.val2
		  << T('-') << std::setw(4) << data.unpacked.val3
		  << T('-') << std::setw(4) << data.unpacked.val4
		  << T('-') << std::setw(2) << static_cast<uint16_t>(data.unpacked.val5.unpacked.v1)
		  << std::setw(2) << static_cast<uint16_t>(data.unpacked.val5.unpacked.v2)
		  << std::setw(2) << static_cast<uint16_t>(data.unpacked.val5.unpacked.v3)
		  << std::setw(2) << static_cast<uint16_t>(data.unpacked.val5.unpacked.v4)
		  << std::setw(2) << static_cast<uint16_t>(data.unpacked.val5.unpacked.v5)
		  << std::setw(2) << static_cast<uint16_t>(data.unpacked.val5.unpacked.v6);
		str = s.str();
	}

	std::string toString() const
	{
		std::string str;
		toString(str);
		return str;
	}

	union ERUUIDData getData() const
	{
		return data;
	}

	const uint8_t *unpackedData() const
	{
		return data.raw;
	}

	void getPackedData(uint64_t &outPacked1, uint64_t &outPacked2)
	{
		outPacked1 = data.packed.v1;
		outPacked2 = data.packed.v2;
	}

	template<typename T, typename Traits>
	friend std::basic_ostream<T, Traits> &operator <<(std::basic_ostream<T, Traits> &, const ERUUID &);
};

template<typename T, typename Traits>
std::basic_ostream<T, Traits> &operator <<(std::basic_ostream<T, Traits> &strm, const ERUUID &uuid)
{
	std::basic_string<T, Traits> str;
	uuid.toString(str);
	strm << str;
	return strm;
}

template<typename T>
class ERServiceUUID
{
public:
	static const ERUUID uuid;
};

template<typename T>
const ERUUID ERServiceUUID<T>::uuid;
