#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <iostream>
#include <string>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include "ERUUID.h"

#include "MTRandom.h"

using namespace std;

int wmain()
{
	char gen[50] = {'\0', };

	init_genrand(static_cast<unsigned long>(time(nullptr)));

	ERUUID uuid(
		genrand_int32(),
		static_cast<unsigned short>(genrand_real2() * 0x10000),
		0x4000 | static_cast<unsigned short>(genrand_real2() * 0x1000),
		((static_cast<unsigned short>(genrand_real2() * 4) + 8) << 12) | static_cast<unsigned short>(genrand_real2() * 0x1000),
		static_cast<unsigned char>(genrand_real2() * 0x100),
		static_cast<unsigned char>(genrand_real2() * 0x100),
		static_cast<unsigned char>(genrand_real2() * 0x100),
		static_cast<unsigned char>(genrand_real2() * 0x100),
		static_cast<unsigned char>(genrand_real2() * 0x100),
		static_cast<unsigned char>(genrand_real2() * 0x100)
	);

	union ERUUIDData data = uuid.getData();

	printf(
		"ERUUID(0x%08X, 0x%04X, 0x%04X, 0x%04X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X)\n",
		data.unpacked.val1, data.unpacked.val2, data.unpacked.val3, data.unpacked.val4,
		data.unpacked.val5.unpacked.v1, data.unpacked.val5.unpacked.v2, data.unpacked.val5.unpacked.v3,
		data.unpacked.val5.unpacked.v4, data.unpacked.val5.unpacked.v5, data.unpacked.val5.unpacked.v6
	);
	printf("// %s\n", uuid.toString().c_str());

	return 0;
}
