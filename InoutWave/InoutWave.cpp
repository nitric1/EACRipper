#define WIN32_LEAN_AND_MEAN
#include "../Common/Component/ERComponent.h"

class InoutWaveEntrypoint : public IERComponentEntrypoint
{
public:
	virtual bool onInit();
	virtual bool onUninit();
};

bool InoutWaveEntrypoint::onInit()
{
	return true;
}

bool InoutWaveEntrypoint::onUninit()
{
	return true;
}

ERComponentEntrypointFactory<InoutWaveEntrypoint> entry;

DECLARE_COMPONENT(L"InoutWave", L"1.0.0");
