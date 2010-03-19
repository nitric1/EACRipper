#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>

#include "Component/ERComponent.h"

using namespace ERComponent;

class InoutWaveEntrypoint;
class InWave;
class OutWave;

class InoutWaveEntrypoint
{
public:
	bool onInit();
	bool onUninit();
};

bool InoutWaveEntrypoint::onInit()
{
	MusicDecoderFactory<InWave> waveDecoderFac;
	MusicEncoderFactory<OutWave> waveEncoderFac;

	return true;
}

bool InoutWaveEntrypoint::onUninit()
{
	return true;
}

class InWave
{
public:
};

class OutWave
{
public:
};

EntrypointRegister<InoutWaveEntrypoint> entry;

DECLARE_COMPONENT(L"InoutWave", L"1.0.0");
