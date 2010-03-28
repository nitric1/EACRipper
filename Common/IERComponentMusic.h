#pragma once

#include "ERUUID.h"

#include "IERAllocator.h"

typedef struct _DecoderInformation
{
	const wchar_t *name; // e.g. "RIFF Audio (Wave)"
	const wchar_t *extension; // e.g. L"wav;wave"
	const wchar_t *mime; // e.g. L"audio/wav;audio/wave;audio/x-wav;audio/vnd.wave"
} DecoderInformation;

typedef struct _EncoderInformation
{
	const wchar_t *name; // e.g. "RIFF Audio (Wave)"
	const wchar_t *extension; // e.g. L"wav"
	const wchar_t *mime; // e.g. L"audio/wav"
} EncoderInformation;

class IERComponentMusicDecoder
{
public:
	/**
	 * Destructor for safe deriving.
	 */
	virtual ~IERComponentMusicDecoder() = 0 {}

public:
	/**
	 * Return the information of the decoder.
	 * @return DecoderInformation which includes name, extension, some other information.
	 */
	virtual const DecoderInformation &getInfo() const = 0;

	/**
	 * Set a stream reader of music.
	 * @param stream A pointer to a stream reader of music.
	 * @return true if a reader is set correctly, false otherwise.
	 */
	virtual bool setStream(IERStreamReader *stream) = 0;

	/**
	 * Open a file as read mode.
	 * @param name File name to be opened.
	 * @return true if the file is opened correctly, false otherwise.
	 */
	virtual uint8_t getChannels() const = 0;

	/**
	 * Return bits per sample of the opened file.
	 * @return The number of bits per sample.
	 */
	virtual uint8_t getBitsPerSample() const = 0;

	/**
	 * Return sampling rate of the opened file.
	 * @return Sampling rate value in Hz.
	 */
	virtual uint32_t getSamplingRate() const = 0;

	/**
	 * Return music length of the opened file.
	 * @return Length in millisecond.
	 */
	virtual uint32_t getLength() const = 0;

	/**
	 * Return required buffer size in the section [startMillisec, endMillisec).
	 * @param startMillisec Start of the section in millisecond.
	 * @param endMillisec End of the section in millisecond.
	 * @return Required buffer size in byte.
	 */
	virtual uint32_t getSize(uint32_t startMillisec, uint32_t endMillisec) const
	{
		return static_cast<uint32_t>(getChannels()) * static_cast<uint32_t>(getBitsPerSample() / 8) * static_cast<uint32_t>(static_cast<uint64_t>(endMillisec - startMillisec) * getSamplingRate() / 1000);
	}
};

class IERComponentInCueMusicDecoder : public IERComponentMusicDecoder
{
public:
	/**
	 * Destructor for safe deriving.
	 */
	virtual ~IERComponentInCueMusicDecoder() = 0 {}

public:
	/**
	 * Return required buffer length about wchar_t of inCue string.
	 * @param converter String converter to convert non-wchar_t string.
	 * @return Required buffer length.
	 */
	virtual size_t getInCueLength(IERServiceStringConverter *converter) = 0;

	/**
	 * Return required buffer length about wchar_t of inCue string.
	 * @param buffer Buffer in which inCue string is to be stored.
	 * @param bufferSize The buffer's length in characters.
	 * @param converter String converter to convert non-wchar_t string.
	 * @return true if succeeded, false otherwise.
	 */
	virtual bool readInCue(wchar_t *buffer, size_t bufferSize, IERServiceStringConverter *converter) = 0;
};

class IERComponentMusicEncoder
{
public:
	/**
	 * Destructor for safe deriving.
	 */
	virtual ~IERComponentMusicEncoder() = 0 {}

public:
	/**
	 * Return the information of the encoder.
	 * @return EncoderInformation which includes name, extension, some other information.
	 */
	virtual const EncoderInformation &getInfo() const = 0;
};

class IERServiceMusicDecoderRegister
{
public:
	virtual ~IERServiceMusicDecoderRegister() = 0 {}

public:
	virtual bool registDecoder(IERAllocator *) = 0;
};

class IERServiceInCueMusicDecoderRegister
{
public:
	virtual ~IERServiceInCueMusicDecoderRegister() = 0 {}

public:
	virtual bool registInCueDecoder(IERAllocator *) = 0;
};

class IERServiceMusicEncoderRegister
{
public:
	virtual ~IERServiceMusicEncoderRegister() = 0 {}

public:
	virtual bool registEncoder(IERAllocator *) = 0;
};

template<>
const ERUUID ERServiceUUID<IERServiceMusicDecoderRegister>::uuid
	= ERUUID(0x25C12300, 0xE3D9, 0x4C1F, 0x8496, 0xF8, 0xF1, 0x22, 0x30, 0x5A, 0x6C);
// 25C12300-E3D9-4C1F-8496-F8F122305A6C

template<>
const ERUUID ERServiceUUID<IERServiceInCueMusicDecoderRegister>::uuid
	= ERUUID(0xD13C888F, 0xB4F2, 0x46AA, 0xA7CE, 0x85, 0x66, 0x42, 0x5E, 0xE6, 0x35);
// D13C888F-B4F2-46AA-A7CE-8566425EE635

template<>
const ERUUID ERServiceUUID<IERServiceMusicEncoderRegister>::uuid
	= ERUUID(0xD15C7DA9, 0x2E12, 0x4552, 0xB401, 0x5A, 0x2A, 0x3A, 0x40, 0xB8, 0x21);
// D15C7DA9-2E12-4552-B401-5A2A3A40B821