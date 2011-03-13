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
		uint8_t chunkCC[4];
		uint32_t chunkID;
	};
	uint32_t chunkSize;
	union
	{
		uint8_t formatCC[4];
		uint32_t formatID;
	};
	union
	{
		uint8_t subchunkCC[4];
		uint32_t subchunkID;
	};
	uint32_t subchunkSize;
	uint16_t audioFormat;
	uint16_t channels;
	uint32_t samplingRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample;
	union
	{
		uint8_t dataCC[4];
		uint32_t dataID;
	};
	uint32_t dataSize;
} WaveHeader;
#pragma pack(pop)

static const uint32_t waveChunkID = 0x46464952u;
static const uint32_t waveFormatID = 0x45564157u;
static const uint32_t waveSubchunkID = 0x20746d66u;
static const uint32_t waveDataID = 0x61746164u;

class InWave : public IERComponentMusicDecoder
{
public:
	static const DecoderInformation info;

private:
	IERStreamReader *reader;
	WaveHeader header;
	unsigned samples;

public:
	virtual ~InWave();

public:
	virtual const DecoderInformation &getInfo() const;
	virtual bool setStream(IERStreamReader *);
	virtual bool close();
	virtual uint8_t getChannels() const;
	virtual uint8_t getBitsPerSample() const;
	virtual uint32_t getSamplingRate() const;
	virtual uint32_t getLength() const;
	virtual size_t read(uint32_t, uint32_t, void *, size_t);
	virtual bool readSplit(uint32_t, uint32_t, void *, size_t, size_t *, uint64_t *);

private:
	bool seek(uint32_t);
};

const DecoderInformation InWave::info =
{
	L"RIFF Audio (Wave)",
	L"wav;wave",
	L"audio/wav;audio/wave;audio/x-wav;audio/vnd.wave",
	2,
	reinterpret_cast<const uint8_t *>("\x4D\x5A")
};

InWave::~InWave()
{
	close();
}

const DecoderInformation &InWave::getInfo() const
{
	return info;
}

bool InWave::setStream(IERStreamReader *stream)
{
	reader = stream;
	if(!reader->usable())
	{
		close();
		return false;
	}

	if(reader->read(&header, sizeof(header)) != sizeof(header))
	{
		close();
		return false;
	}

	header.chunkID = EREndian::L2N(header.chunkID);
	header.chunkSize = EREndian::L2N(header.chunkSize);
	header.formatID = EREndian::L2N(header.formatID);
	header.subchunkID = EREndian::L2N(header.subchunkID);
	header.subchunkSize = EREndian::L2N(header.subchunkSize);
	header.audioFormat = EREndian::L2N(header.audioFormat);
	header.channels = EREndian::L2N(header.channels);
	header.samplingRate = EREndian::L2N(header.samplingRate);
	header.byteRate = EREndian::L2N(header.byteRate);
	header.blockAlign = EREndian::L2N(header.blockAlign);
	header.bitsPerSample = EREndian::L2N(header.bitsPerSample);
	header.dataID = EREndian::L2N(header.dataID);
	header.dataSize = EREndian::L2N(header.dataSize);

	if(header.chunkID != waveChunkID || header.formatID != waveFormatID || header.subchunkID != waveSubchunkID || header.dataID != waveDataID || header.audioFormat != WAVE_FORMAT_PCM)
	{
		close();
		return false;
	}

	samples = header.dataSize / header.channels / (header.bitsPerSample / 8);

	return true;
}

bool InWave::close()
{
	reader = nullptr;
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

	size_t dataSize = getDataSize(start, end);
	if(bufferSize > dataSize)
		bufferSize = dataSize;

	if(!seek(start))
		return numeric_limits<size_t>::max();

	return reader->read(buffer, bufferSize);
}

bool InWave::readSplit(uint32_t start, uint32_t end, void *buffer, size_t bufferSize, size_t *readSize, uint64_t *section)
{
	if(*section == 0 && !seek(start)) // first read
		return false;

	size_t leftSize = getDataSize(start, end) - static_cast<size_t>(*section);
	if(bufferSize >= leftSize)
		bufferSize = leftSize;

	*readSize = reader->read(buffer, bufferSize);
	if(*readSize == numeric_limits<size_t>::max() || *readSize >= leftSize)
		return false;

	*section += *readSize;

	return true;
}

bool InWave::seek(uint32_t sec)
{
	return reader->seek(getDataSize(0, sec) + sizeof(header), BEGIN);
}

class OutWave : public IERComponentMusicEncoder
{
public:
	static const EncoderInformation info;

private:
	IERStreamWriter *writer;
	WaveHeader header;

public:
	virtual ~OutWave();

public:
	virtual const EncoderInformation &getInfo() const;
	virtual bool setStream(IERStreamWriter *, uint8_t, uint8_t, uint32_t, uint32_t);
	virtual bool close();
	virtual uint8_t getChannels() const;
	virtual uint8_t getBitsPerSample() const;
	virtual uint32_t getSamplingRate() const;
	virtual bool write(const void *, size_t);
	virtual void setTag(const wchar_t *, const wchar_t *);
	virtual bool setCoverArt(IERStreamReader *, const wchar_t *);
};

const EncoderInformation OutWave::info =
{
	L"RIFF Audio (Wave)",
	L"wav",
	L"audio/wav"
};

OutWave::~OutWave()
{
	close();
}

const EncoderInformation &OutWave::getInfo() const
{
	return info;
}

bool OutWave::setStream(IERStreamWriter *stream, uint8_t channels, uint8_t bitsPerSample, uint32_t samplingRate, uint32_t totalSize)
{
	writer = stream;
	if(!writer->usable())
		return false;

	memset(&header, 0, sizeof(header));

	header.chunkID = waveChunkID;
	header.chunkSize = 36 + totalSize;
	header.formatID = waveFormatID;
	header.subchunkID = waveSubchunkID;
	header.subchunkSize = 16;
	header.audioFormat = WAVE_FORMAT_PCM;
	header.channels = channels;
	header.samplingRate = samplingRate;
	header.byteRate = samplingRate * channels * bitsPerSample / 8;
	header.blockAlign = channels * bitsPerSample / 8;
	header.bitsPerSample = bitsPerSample;
	header.dataID = waveDataID;
	header.dataSize = totalSize;

	header.chunkID = EREndian::N2L(header.chunkID);
	header.chunkSize = EREndian::N2L(header.chunkSize);
	header.formatID = EREndian::N2L(header.formatID);
	header.subchunkID = EREndian::N2L(header.subchunkID);
	header.subchunkSize = EREndian::N2L(header.subchunkSize);
	header.audioFormat = EREndian::N2L(header.audioFormat);
	header.channels = EREndian::N2L(header.channels);
	header.samplingRate = EREndian::N2L(header.samplingRate);
	header.byteRate = EREndian::N2L(header.byteRate);
	header.blockAlign = EREndian::N2L(header.blockAlign);
	header.bitsPerSample = EREndian::N2L(header.bitsPerSample);
	header.dataID = EREndian::N2L(header.dataID);
	header.dataSize = EREndian::N2L(header.dataSize);

	if(writer->write(&header, sizeof(header)) != sizeof(header))
	{
		close();
		return false;
	}

	return true;
}

bool OutWave::close()
{
	writer = nullptr;
	return true;
}

uint8_t OutWave::getChannels() const
{
	return static_cast<uint8_t>(header.channels);
}

uint8_t OutWave::getBitsPerSample() const
{
	return static_cast<uint8_t>(header.bitsPerSample);
}

uint32_t OutWave::getSamplingRate() const
{
	return header.samplingRate;
}

bool OutWave::write(const void *data, size_t dataSize)
{
	return writer->write(data, dataSize) != dataSize;
}

void OutWave::setTag(const wchar_t *name, const wchar_t *value)
{
}

bool OutWave::setCoverArt(IERStreamReader *image, const wchar_t *imageMime)
{
	return false;
}

EntrypointRegister<InoutWaveEntrypoint> entry;

DECLARE_COMPONENT(L"InoutWave", L"1.0.0");
