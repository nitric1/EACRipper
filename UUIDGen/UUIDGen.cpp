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

	sprintf(gen, "%08X-%04hX-4%03hX-%hX%03hX-%08X%04hX",
		genrand_int32(),
		static_cast<unsigned short>(genrand_real2() * 0x10000),
		static_cast<unsigned short>(genrand_real2() * 0x1000),
		static_cast<unsigned short>(genrand_real2() * 4) + 8,
		static_cast<unsigned short>(genrand_real2() * 0x1000),
		genrand_int32(),
		static_cast<unsigned short>(genrand_real2() * 0x10000)
		);

	ERUUID uuid = gen;

	union ERUUIDData data = uuid.getData();

	printf("ERUUID(0x%08X, 0x%04X, 0x%04X, 0x%04X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X)\n",
		data.val1, data.val2, data.val3, data.val4,
		data.val5_1, data.val5_2, data.val5_3, data.val5_4, data.val5_5, data.val5_6
		);
	printf("// %s\n", gen);

	return 0;
}
