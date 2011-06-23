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
/// UUID data for ERUUID class.
/// @author Wondong LEE
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

/// An UUID(Universally unique identifier) class.
/// @author Wondong LEE
class ERUUID
{
private:
	union ERUUIDData data;

public:
	static const size_t unpackedSize = 16; ///< Size of entire data.

public:
	/// Default constructor.
	/// @author Wondong LEE
	ERUUID() : data() {}

	/// Copy constructor.
	/// @author Wondong LEE
	/// @param uuid Data to be copied.
	ERUUID(const ERUUID &uuid) : data(uuid.data) {}

	/// Copy constructor.
	/// @author Wondong LEE
	/// @param idata Data to be copied.
	ERUUID(const ERUUIDData &idata) : data(idata) {}

	/// Constructs by copying preset UUID of class <code>T</code>.
	/// @author Wondong LEE
	/// @param preset Preset data to be copied.
	template<typename T>
	ERUUID(const ERServiceUUID<T> &preset) : data(preset.uuid.data) {}

	/// Constructs using packed UUID data.
	/// @author Wondong LEE
	/// @param data The 16-bit packed UUID data.
	explicit ERUUID(const uint8_t *data) : data()
	{
		this->data.packed.v1 = *reinterpret_cast<const uint64_t *>(data);
		this->data.packed.v2 = *reinterpret_cast<const uint64_t *>(data + 8);
	}

	/// Constructs using unpacked UUID data.
	/// @author Wondong LEE
	/// @param val1   The first 32-bit UUID field.
	/// @param val2   The second 16-bit UUID field.
	/// @param val3   The third 16-bit UUID field.
	/// @param val4   The fourth 16-bit UUID field.
	/// @param val5_1 The first 8-bit data of the fifth 48-bit UUID field.
	/// @param val5_2 The second 8-bit data of the fifth 48-bit UUID field.
	/// @param val5_3 The third 8-bit data of the fifth 48-bit UUID field.
	/// @param val5_4 The fourth 8-bit data of the fifth 48-bit UUID field.
	/// @param val5_5 The fifth 8-bit data of the fifth 48-bit UUID field.
	/// @param val5_6 The sixth 8-bit data of the fifth 48-bit UUID field.
	explicit ERUUID(uint32_t val1, uint16_t val2, uint16_t val3, uint16_t val4,
		uint8_t val5_1, uint8_t val5_2, uint8_t val5_3, uint8_t val5_4, uint8_t val5_5, uint8_t val5_6)
		: data()
	{
		data.unpacked.val1 = val1;
		data.unpacked.val2 = val2;
		data.unpacked.val3 = val3;
		data.unpacked.val4 = val4;
		data.unpacked.val5.unpacked.v1 = val5_1;
		data.unpacked.val5.unpacked.v2 = val5_2;
		data.unpacked.val5.unpacked.v3 = val5_3;
		data.unpacked.val5.unpacked.v4 = val5_4;
		data.unpacked.val5.unpacked.v5 = val5_5;
		data.unpacked.val5.unpacked.v6 = val5_6;
	}

	/// Constructs using the unpacked first 80-bit UUID data and the packed next 48-bit UUID data.
	/// @author Wondong LEE
	/// @param val1 The first 32-bit UUID field.
	/// @param val2 The second 16-bit UUID field.
	/// @param val3 The third 16-bit UUID field.
	/// @param val4 The fourth 16-bit UUID field.
	/// @param val5 The fifth packed 48-bit UUID field.
	explicit ERUUID(uint32_t val1, uint16_t val2, uint16_t val3, uint16_t val4, const uint8_t *val5) : data()
	{
		data.unpacked.val1 = val1;
		data.unpacked.val2 = val2;
		data.unpacked.val3 = val3;
		data.unpacked.val4 = val4;
		data.unpacked.val5.packed.v1 = *reinterpret_cast<const uint32_t *>(val5);
		data.unpacked.val5.packed.v2 = *reinterpret_cast<const uint16_t *>(val5 + 4);
	}

	/// Constructs using a string representation.
	/// @author Wondong LEE
	/// @param str The string.
	ERUUID(const char *str) : data() { parseString(str); }

	/// Constructs using a string representation.
	/// @author Wondong LEE
	/// @param str The string.
	ERUUID(const wchar_t *str) : data() { parseString(str); }

public:
	/// Assignment operator.
	/// @author Wondong LEE
	/// @param uuid Data to be copied.
	/// @return This object after assigning.
	ERUUID &operator =(const ERUUID &uuid)
	{
		data.packed = uuid.data.packed;
		return *this;
	}

	/// Assignment operator.
	/// @author Wondong LEE
	/// @param uuid Data to be copied.
	/// @return This object after assigning.
	ERUUID &operator =(const ERUUIDData &idata)
	{
		data = idata;
		return *this;
	}

	/// Assignment operator that copies data from preset UUID of class <code>T</code>.
	/// @author Wondong LEE
	/// @param uuid Preset data to be copied.
	/// @return This object after assigning.
	template<typename T>
	ERUUID &operator =(const ERServiceUUID<T> &preset)
	{
		*this = preset.uuid;
		return *this;
	}

	/// Assignment operator that uses a string representation.
	/// @author Wondong LEE
	/// @param str The string.
	/// @return This object after assigning.
	ERUUID &operator =(const char *str)
	{
		parseString(str);
		return *this;
	}

	/// Assignment operator that uses a string representation.
	/// @author Wondong LEE
	/// @param str The string.
	/// @return This object after assigning.
	ERUUID &operator =(const wchar_t *str)
	{
		parseString(str);
		return *this;
	}

	/// Equality operator.
	/// @author Wondong LEE
	/// @param uuid The UUID to compare.
	/// @return <code>true</code> if <code>uuid</code> is equal to this object.
	bool operator ==(const ERUUID &uuid) const
	{
		return data.packed.v1 == uuid.data.packed.v1 && data.packed.v2 == uuid.data.packed.v2;
	}

	/// Not equality operator.
	/// @author Wondong LEE
	/// @param uuid The UUID to compare.
	/// @return <code>true</code> if <code>uuid</code> is not equal to this object.
	bool operator !=(const ERUUID &uuid) const
	{
		return !(*this == uuid);
	}

	/// Casting operator that returns read-only unpacked data.
	/// @author Wondong LEE
	/// @return The read-only unpacked data.
	/// @see #unpackedData
	operator const uint8_t *() const
	{
		return unpackedData();
	}

	/// ERUUIDData casting operator.
	/// @author Wondong LEE
	/// @return This object's data.
	/// @see ERUUIDData
	/// @see #getData
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
	/// Converts this object into a string representation.
	/// @author Wondong LEE
	/// @param [out] str The string buffer.
	/// @param buflen	 The buffer length of <code>str</code>.
	void toString(char *str, size_t buflen) const
	{
		std::string s;
		toString(s);
		strcpy_s(str, buflen, s.c_str());
	}

	/// Converts this object into a string representation.
	/// @author Wondong LEE
	/// @param [out] str The string buffer.
	/// @param buflen	 The buffer length of <code>str</code>.
	void toString(wchar_t *str, size_t buflen) const
	{
		std::wstring s;
		toString(s);
		wcscpy_s(str, buflen, s.c_str());
	}

	/// Converts this object into a string representation.
	/// @author Wondong LEE
	/// @param [out] str A string representation of this object.
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

	/// Converts this object into a string representation.
	/// @author Wondong LEE
	/// @return A string representation of this object.
	std::string toString() const
	{
		std::string str;
		toString(str);
		return str;
	}

	/// Gets this object's data.
	/// @author Wondong LEE
	/// @return This object's data.
	union ERUUIDData getData() const
	{
		return data;
	}

	/// Gets read-only unpacked data.
	/// @author Wondong LEE
	/// @return The read-only unpacked data.
	const uint8_t *unpackedData() const
	{
		return data.raw;
	}

	/// Gets packed data.
	/// @author Wondong LEE
	/// @param [out] outPacked1 The first 64-bit of the packed data.
	/// @param [out] outPacked2 The second 64-bit of the packed data.
	void getPackedData(uint64_t &outPacked1, uint64_t &outPacked2)
	{
		outPacked1 = data.packed.v1;
		outPacked2 = data.packed.v2;
	}

	template<typename T, typename Traits>
	friend std::basic_ostream<T, Traits> &operator <<(std::basic_ostream<T, Traits> &, const ERUUID &);
};

/// Output operator for <code>std::ostream</code>. Outputs a string representation of <code>uuid</code>.
/// @param strm The stream object.
/// @param uuid The UUID object to be output.
/// @return The stream object after output <code>uuid</code>.
template<typename T, typename Traits>
std::basic_ostream<T, Traits> &operator <<(std::basic_ostream<T, Traits> &strm, const ERUUID &uuid)
{
	std::basic_string<T, Traits> str;
	uuid.toString(str);
	strm << str;
	return strm;
}

/// UUID data of service interface <code>T</code>.
/// @author Wondong LEE
template<typename T>
class ERServiceUUID
{
public:
	static const ERUUID uuid; ///< The UUID data of {@code T}.
};

template<typename T>
const ERUUID ERServiceUUID<T>::uuid;
