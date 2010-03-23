#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <algorithm>

using namespace std;

#include <windows.h>
#include <mmsystem.h>

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
	// MessageBoxW(ApplicationInfo::instance().getApp().getWindow(), L"Hello", L"World", MB_OK);

	MusicDecoderFactory<InWave, InWave> waveDecoderFac;
	MusicEncoderFactory<OutWave, OutWave> waveEncoderFac;

	return true;
}

bool InoutWaveEntrypoint::onUninit()
{
	return true;
}

#pragma pack(push, 2)
typedef struct _WaveHeader
{
	union
	{
		unsigned char chunkText[4];
		unsigned long chunkID;
	};
	unsigned long chunkSize;
	union
	{
		unsigned char formatText[4];
		unsigned long formatID;
	};
	union
	{
		unsigned char subchunkText[4];
		unsigned long subchunkID;
	};
	unsigned long subchunkSize;
	unsigned short audioFormat;
	unsigned short channels;
	unsigned long samplingRate;
	unsigned long byteRate;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	union
	{
		unsigned char dataText[4];
		unsigned long dataID;
	};
	unsigned long dataSize;
} WaveHeader;
#pragma pack(pop)

class InWave : public IERComponentMusicDecoder
{
public:
	static const int32_t waveChunkID;
	static const int32_t waveFormatID;
	static const int32_t waveSubchunkID;
	static const int32_t waveDataID;
	static const DecoderInformation info;

private:
	IERStreamReader *reader;
	WaveHeader header;
	unsigned samples;

public:
	virtual const DecoderInformation &getInfo() const;
	virtual bool setStream(IERStreamReader *);
	virtual uint8_t getChannels() const;
	virtual uint8_t getBitsPerSample() const;
	virtual uint32_t getSamplingRate() const;
	virtual uint32_t getLength() const;
	virtual size_t read(uint32_t, uint32_t, void *, size_t);
	virtual bool readSplit(uint32_t, uint32_t, void *, size_t, size_t *, uint64_t *);
};

const int32_t InWave::waveChunkID = 0x46464952;
const int32_t InWave::waveFormatID = 0x45564157;
const int32_t InWave::waveSubchunkID = 0x20746d66;
const int32_t InWave::waveDataID = 0x61746164;
const DecoderInformation InWave::info =
{
	L"RIFF Audio (Wave)",
	L"wav;wave",
	L"audio/wav;audio/wave;audio/x-wav;audio/vnd.wave"
};

const DecoderInformation &InWave::getInfo() const
{
	return info;
}

bool InWave::setStream(IERStreamReader *stream)
{
	reader = stream;
	if(!reader->usable())
		return false;

	if(reader->read(&header, sizeof(header)) != sizeof(header))
		return false;

	if(header.chunkID != waveChunkID || header.formatID != waveFormatID || header.subchunkID != waveSubchunkID || header.dataID != waveDataID)
		return false;

	if(header.audioFormat != WAVE_FORMAT_PCM)
		return false;

	samples = header.dataSize / header.channels / (header.bitsPerSample / 8);

	return true;
}

uint8_t InWave::getChannels() const
{
	return static_cast<uint8_t>(header.channels);
}

uint8_t InWave::getBitsPerSample() const
{
	return static_cast<uint8_t>(header.bitsPerSample);
}

uint32_t InWave::getSamplingRate() const
{
	return header.samplingRate;
}

uint32_t InWave::getLength() const
{
	return static_cast<uint32_t>(static_cast<uint64_t>(samples) * 1000 / getSamplingRate());
}

size_t InWave::read(uint32_t start, uint32_t end, void *buffer, size_t bufferSize)
{
	if(start >= end)
		return 0;

	return 0;
}

bool InWave::readSplit(uint32_t start, uint32_t end, void *buffer, size_t bufferSize, size_t *readSize, uint64_t *section)
{
	if(*section == 0) // first read
	{
	}

	return false;
}

class OutWave : public IERComponentMusicEncoder
{
public:
	static const EncoderInformation info;

public:
	virtual const EncoderInformation &getInfo() const;
	virtual bool setStream(IERStreamWriter *);
};

const EncoderInformation OutWave::info =
{
	L"RIFF Audio (Wave)",
	L"wav",
	L"audio/wav"
};

const EncoderInformation &OutWave::getInfo() const
{
	return info;
}

bool OutWave::setStream(IERStreamWriter *stream)
{
	return false;
}

EntrypointRegister<InoutWaveEntrypoint> entry;

DECLARE_COMPONENT(L"InoutWave", L"1.0.0");
