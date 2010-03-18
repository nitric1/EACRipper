#pragma once

#include <cstdio>
#include <cstdlib>

#define __STDC_FORMAT_MACROS
#include <cinttypes>

#include <iostream>
#include <algorithm>

#include <boost/detail/endian.hpp>

template<typename T>
class ERServiceUUID;

class ERUUID
{
private:
#pragma pack(push, 1)
	union
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
					uint8_t val5_1;
					uint8_t val5_2;
					uint8_t val5_3;
					uint8_t val5_4;
					uint8_t val5_5;
					uint8_t val5_6;
				};
				uint8_t val5[6];
				struct
				{
					uint32_t packedVal5_1;
					uint16_t packedVal5_2;
				};
			};
		};
		uint8_t unpacked[16];
		struct
		{
			uint64_t packed1;
			uint64_t packed2;
		};
	};
#pragma pack(pop)

public:
	static const size_t unpackedSize = 16;

private:
	static const char hexLookup[16];

public:
	ERUUID() : packed1(), packed2() {}
	ERUUID(const ERUUID &uuid) : packed1(uuid.packed1), packed2(uuid.packed2) {}
	template<typename T>
	ERUUID(const ERServiceUUID<T> &preset) : ERUUID(preset.uuid) {}
	explicit ERUUID(const uint8_t *data) : packed1(*reinterpret_cast<const uint64_t *>(data)), packed2(*reinterpret_cast<const uint64_t *>(data + 8)) {}
	explicit ERUUID(uint32_t ival1, uint16_t ival2, uint16_t ival3, uint16_t ival4,
		uint8_t ival5_1, uint8_t ival5_2, uint8_t ival5_3, uint8_t ival5_4, uint8_t ival5_5, uint8_t ival5_6)
		: val1(ival1), val2(ival2), val3(ival3), val4(ival4),
		  val5_1(ival5_1), val5_2(ival5_2), val5_3(ival5_3), val5_4(ival5_4), val5_5(ival5_5), val5_6(ival5_6) {}
	explicit ERUUID(uint32_t ival1, uint16_t ival2, uint16_t ival3, uint16_t ival4, const uint8_t *ival5)
		: val1(ival1), val2(ival2), val3(ival3), val4(ival4), packedVal5_1(*reinterpret_cast<const uint32_t *>(ival5)), packedVal5_2(*reinterpret_cast<const uint16_t *>(ival5 + 4)) {}
	ERUUID(const char *str) : packed1(), packed2() { parseString(str); }
	ERUUID(const wchar_t *str) : packed1(), packed2() { parseString(str); }

public:
	ERUUID &operator =(const ERUUID &uuid)
	{
		packed1 = uuid.packed1;
		packed2 = uuid.packed2;
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
		return packed1 == uuid.packed1 && packed2 == uuid.packed2;
	}

	bool operator !=(const ERUUID &uuid) const
	{
		return !(*this == uuid);
	}

	operator const uint8_t *() const
	{
		return unpackedData();
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
		uint8_t data;
		for(size_t i = 0; *str != '\0' && i < unpackedSize; ++ str)
		{
			if((*str >= '0' && *str <= '9') || (*str >= 'A' && *str <= 'F') || (*str >= 'a' && *str <= 'f'))
			{
				if(next)
				{
					unpacked[i ++] = (data << 4) | hexDigit(*str);
					next = false;
				}
				else
				{
					data = hexDigit(*str);
					next = true;
				}
			}
		}

#ifdef BOOST_LITTLE_ENDIAN
		std::swap(unpacked[0], unpacked[3]);
		std::swap(unpacked[1], unpacked[2]);
		std::swap(unpacked[4], unpacked[5]);
		std::swap(unpacked[6], unpacked[7]);
		std::swap(unpacked[8], unpacked[9]);
#endif

		return true;
	}

	bool parseString(const wchar_t *str)
	{
		bool next = false;
		uint8_t data;
		for(size_t i = 0; *str != L'\0' && i < unpackedSize; ++ str)
		{
			if((*str >= L'0' && *str <= L'9') || (*str >= L'A' && *str <= L'F') || (*str >= L'a' && *str <= L'f'))
			{
				if(next)
				{
					unpacked[i ++] = (data << 4) | hexDigit(*str);
					next = false;
				}
				else
				{
					data = hexDigit(*str);
					next = true;
				}
			}
		}

#ifdef BOOST_LITTLE_ENDIAN
		std::swap(unpacked[0], unpacked[3]);
		std::swap(unpacked[1], unpacked[2]);
		std::swap(unpacked[4], unpacked[5]);
		std::swap(unpacked[6], unpacked[7]);
		std::swap(unpacked[8], unpacked[9]);
#endif

		return true;
	}

public:
	void toString(char *str) const
	{
		sprintf(str, "%08" PRIX32 "-%04" PRIX16 "-%04" PRIX16 "-%04" PRIX16 "-%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8,
			val1, val2, val3, val4, val5_1, val5_2, val5_3, val5_4, val5_5, val5_6);
	}

	void toString(wchar_t *str) const
	{
		char buf[37];
		toString(buf);
		mbstowcs(str, buf, 37);
	}

	const uint8_t *unpackedData() const
	{
		return unpacked;
	}

	void getPackedData(uint64_t &outPacked1, uint64_t &outPacked2)
	{
		outPacked1 = packed1;
		outPacked2 = packed2;
	}
};

const char ERUUID::hexLookup[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

template<typename T>
class ERServiceUUID
{
public:
	static const ERUUID uuid;
};

template<typename T>
const ERUUID ERServiceUUID<T>::uuid;
