#pragma once

#include <cinttypes>

#include <boost/detail/endian.hpp>

class EREndian
{
private:
	EREndian();
	~EREndian();

public:
	static uint16_t swap(uint16_t v)
	{ return (v << 8) | (v >> 8); }
	static uint32_t swap(uint32_t v)
	{ return (v << 24) | ((v & 0xFF00u) << 8) | ((v & 0xFF0000u) >> 8) | (v >> 24); }
	static uint64_t swap(uint64_t v)
	{ return (v << 56) | ((v & 0xFF00ull) << 40) | ((v & 0xFF0000ull) << 24) |
			 ((v & 0xFF000000ull) << 8) | ((v & 0xFF00000000ull) >> 8) |
			 ((v & 0xFF0000000000ull) >> 24) | ((v & 0xFF000000000000ull) >> 40) |
			 (v >> 56); }

#ifdef BOOST_BIG_ENDIAN
	static uint16_t N2L(uint16_t v) { return swap(v); }
	static uint32_t N2L(uint32_t v) { return swap(v); }
	static uint64_t N2L(uint64_t v) { return swap(v); }
	
	static uint16_t N2B(uint16_t v) { return v; }
	static uint32_t N2B(uint32_t v) { return v; }
	static uint64_t N2B(uint64_t v) { return v; }
#else
	static uint16_t N2L(uint16_t v) { return v; }
	static uint32_t N2L(uint32_t v) { return v; }
	static uint64_t N2L(uint64_t v) { return v; }

	static uint16_t N2B(uint16_t v) { return swap(v); }
	static uint32_t N2B(uint32_t v) { return swap(v); }
	static uint64_t N2B(uint64_t v) { return swap(v); }
#endif
	// Little Endian to Native
	static uint16_t L2N(uint16_t v) { return N2L(v); }
	static uint32_t L2N(uint32_t v) { return N2L(v); }
	static uint64_t L2N(uint64_t v) { return N2L(v); }

	// Big Endian to Native
	static uint16_t B2N(uint16_t v) { return N2B(v); }
	static uint32_t B2N(uint32_t v) { return N2B(v); }
	static uint64_t B2N(uint64_t v) { return N2B(v); }

	// Native to Little Endian
	static int16_t N2L(int16_t v) { return static_cast<int16_t>(N2L(static_cast<uint16_t>(v))); }
	static int32_t N2L(int32_t v) { return static_cast<int32_t>(N2L(static_cast<uint32_t>(v))); }
	static int64_t N2L(int64_t v) { return static_cast<int64_t>(N2L(static_cast<uint64_t>(v))); }

	// Native to Big Endian
	static int16_t N2B(int16_t v) { return static_cast<int16_t>(N2B(static_cast<uint16_t>(v))); }
	static int32_t N2B(int32_t v) { return static_cast<int32_t>(N2B(static_cast<uint32_t>(v))); }
	static int64_t N2B(int64_t v) { return static_cast<int64_t>(N2B(static_cast<uint64_t>(v))); }

	static int16_t L2N(int16_t v) { return static_cast<int16_t>(L2N(static_cast<uint16_t>(v))); }
	static int32_t L2N(int32_t v) { return static_cast<int32_t>(L2N(static_cast<uint32_t>(v))); }
	static int64_t L2N(int64_t v) { return static_cast<int64_t>(L2N(static_cast<uint64_t>(v))); }

	static int16_t B2N(int16_t v) { return static_cast<int16_t>(B2N(static_cast<uint16_t>(v))); }
	static int32_t B2N(int32_t v) { return static_cast<int32_t>(B2N(static_cast<uint32_t>(v))); }
	static int64_t B2N(int64_t v) { return static_cast<int64_t>(B2N(static_cast<uint64_t>(v))); }
};
