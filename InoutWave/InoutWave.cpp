#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>

#include "Component/ERComponent.h"

using namespace ERComponent;

class InoutWaveEntrypoint
{
public:
	bool onInit();
	bool onUninit();
};

bool InoutWaveEntrypoint::onInit()
{
	return true;
}

bool InoutWaveEntrypoint::onUninit()
{
	return true;
}

class InWave
{
};

class OutWave
{
public:
	ERComponentMusicFormat getFormats();
};

EntrypointRegister<InoutWaveEntrypoint> entryFac;
MusicDecoderRegister<InWave> waveDecoderFac;
MusicEncoderRegister<OutWave> waveEncoderFac;

DECLARE_COMPONENT(L"InoutWave", L"1.0.0");
