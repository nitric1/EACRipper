#pragma once

#include <cstdio>
#include <cstdlib>

#include <cinttypes>

#include <string>
#include <iosfwd>
#include <iomanip>
#include <algorithm>

#include <boost/detail/endian.hpp>

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
		data.packed1 = *reinterpret_cast<const uint64_t *>(idata);
		data.packed2 = *reinterpret_cast<const uint64_t *>(idata + 8);
	}
	explicit ERUUID(uint32_t ival1, uint16_t ival2, uint16_t ival3, uint16_t ival4,
		uint8_t ival5_1, uint8_t ival5_2, uint8_t ival5_3, uint8_t ival5_4, uint8_t ival5_5, uint8_t ival5_6)
		: data()
	{
		data.val1 = ival1;
		data.val2 = ival2;
		data.val3 = ival3;
		data.val4 = ival4;
		data.val5_1 = ival5_1;
		data.val5_2 = ival5_2;
		data.val5_3 = ival5_3;
		data.val5_4 = ival5_4;
		data.val5_5 = ival5_5;
		data.val5_6 = ival5_6;
	}
	explicit ERUUID(uint32_t ival1, uint16_t ival2, uint16_t ival3, uint16_t ival4, const uint8_t *ival5) : data()
	{
		data.val1 = ival1;
		data.val2 = ival2;
		data.val3 = ival3;
		data.val4 = ival4;
		data.packedVal5_1 = *reinterpret_cast<const uint32_t *>(ival5);
		data.packedVal5_2 = *reinterpret_cast<const uint16_t *>(ival5 + 4);
	}
	ERUUID(const char *str) : data() { parseString(str); }
	ERUUID(const wchar_t *str) : data() { parseString(str); }

public:
	ERUUID &operator =(const ERUUID &uuid)
	{
		data.packed1 = uuid.data.packed1;
		data.packed2 = uuid.data.packed2;
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
		return data.packed1 == uuid.data.packed1 && data.packed2 == uuid.data.packed2;
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
		uint8_t ndata;
		for(size_t i = 0; *str != '\0' && i < unpackedSize; ++ str)
		{
			if((*str >= '0' && *str <= '9') || (*str >= 'A' && *str <= 'F') || (*str >= 'a' && *str <= 'f'))
			{
				if(next)
				{
					data.unpacked[i ++] = (ndata << 4) | hexDigit(*str);
					next = false;
				}
				else
				{
					ndata = hexDigit(*str);
					next = true;
				}
			}
		}

#ifdef BOOST_LITTLE_ENDIAN
		std::swap(data.unpacked[0], data.unpacked[3]);
		std::swap(data.unpacked[1], data.unpacked[2]);
		std::swap(data.unpacked[4], data.unpacked[5]);
		std::swap(data.unpacked[6], data.unpacked[7]);
		std::swap(data.unpacked[8], data.unpacked[9]);
#endif

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
					data.unpacked[i ++] = (ndata << 4) | hexDigit(*str);
					next = false;
				}
				else
				{
					ndata = hexDigit(*str);
					next = true;
				}
			}
		}

#ifdef BOOST_LITTLE_ENDIAN
		std::swap(data.unpacked[0], data.unpacked[3]);
		std::swap(data.unpacked[1], data.unpacked[2]);
		std::swap(data.unpacked[4], data.unpacked[5]);
		std::swap(data.unpacked[6], data.unpacked[7]);
		std::swap(data.unpacked[8], data.unpacked[9]);
#endif

		return true;
	}

public:
	void toString(char *str, size_t buflen) const
	{
		sprintf_s(str, buflen, "%08" PRIX32 "-%04" PRIX16 "-%04" PRIX16 "-%04" PRIX16 "-%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8,
			data.val1, data.val2, data.val3, data.val4, data.val5_1, data.val5_2, data.val5_3, data.val5_4, data.val5_5, data.val5_6);
	}

	void toString(wchar_t *str, size_t buflen) const
	{
		char buf[37];
		size_t converted;

		toString(buf, 37);

		mbstowcs_s(&converted, str, buflen, buf, 37);
	}

	union ERUUIDData getData() const
	{
		return data;
	}

	const uint8_t *unpackedData() const
	{
		return data.unpacked;
	}

	void getPackedData(uint64_t &outPacked1, uint64_t &outPacked2)
	{
		outPacked1 = data.packed1;
		outPacked2 = data.packed2;
	}

	template<typename T, typename Traits>
	friend std::basic_ostream<T, Traits> &operator <<(std::basic_ostream<T, Traits> &, const ERUUID &);
};

template<typename T, typename Traits>
std::basic_ostream<T, Traits> &operator <<(std::basic_ostream<T, Traits> &strm, const ERUUID &uuid)
{
	strm << std::hex << std::setw(8) << std::setfill(T('0')) << std::uppercase << uuid.data.val1
		<< T('-') << std::setw(4) << uuid.data.val2
		<< T('-') << std::setw(4) << uuid.data.val3
		<< T('-') << std::setw(4) << uuid.data.val4
		<< T('-') << std::setw(2) << static_cast<uint16_t>(uuid.data.val5_1)
		<< std::setw(2) << static_cast<uint16_t>(uuid.data.val5_2)
		<< std::setw(2) << static_cast<uint16_t>(uuid.data.val5_3)
		<< std::setw(2) << static_cast<uint16_t>(uuid.data.val5_4)
		<< std::setw(2) << static_cast<uint16_t>(uuid.data.val5_5)
		<< std::setw(2) << static_cast<uint16_t>(uuid.data.val5_6);

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
