#pragma once

#ifndef _INCLUDE_ERCOMPONENTMUSIC_
#define _INCLUDE_ERCOMPONENTMUSIC_

#include "Component/Factory.h"
#include "IERComponentMusic.h"

namespace ERComponent
{
	template<typename T>
	class MusicDecoderImpl : public IERComponentMusicDecoder
	{
	private:
		T dec;

	public:
		virtual const DecoderInformation &getInfo() { return dec.getInfo(); }
		virtual bool setStream(IERStreamReader *stream) { dec.setStream(stream); return true; }
		virtual uint8_t getChannels() const { return dec.getChannels(); }
		virtual uint8_t getBitsPerSample() const { return dec.getBitsPerSample(); }
		virtual uint32_t getSamplingRate() const { return dec.getSamplingRate(); }
		virtual uint32_t getLength() const { return dec.getLength(); }
		virtual uint32_t getSize(uint32_t startMillisec, uint32_t endMillisec) const { return dec.getSize(startMillisec, endMillisec); }
	};

	template<typename T>
	class InCueMusicDecoderImpl : public IERComponentInCueMusicDecoder
	{
	private:
		T dec;

	public:
		virtual const DecoderInformation &getInfo() { return dec.getInfo(); }
		virtual bool setStream(IERStreamReader *stream) { dec.setStream(stream); return true; }
		virtual uint8_t getChannels() const { return dec.getChannels(); }
		virtual uint8_t getBitsPerSample() const { return dec.getBitsPerSample(); }
		virtual uint32_t getSamplingRate() const { return dec.getSamplingRate(); }
		virtual uint32_t getLength() const { return dec.getLength(); }
		virtual uint32_t getSize(uint32_t startMillisec, uint32_t endMillisec) const { return dec.getSize(startMillisec, endMillisec); }
		virtual size_t getInCueLength(IERServiceStringConverter *converter) { return dec.getInCueLength(converter); }
		virtual bool readInCue(wchar_t *buffer, size_t bufferSize, IERServiceStringConverter *converter) { return dec.readInCue(buffer, bufferSize, converter); }
	};

	template<typename T>
	class MusicEncoderImpl : public IERComponentMusicEncoder
	{
	};

	template<typename T, typename Impl = MusicDecoderImpl<T> >
	class MusicDecoderFactory : public ServiceFactory<IERServiceMusicDecoderRegister>
	{
	public:
		MusicDecoderFactory()
		{
			getPtr()->registDecoder(&Allocator<Impl>::instance());
		}
	};

	template<typename T, typename Impl = InCueMusicDecoderImpl<T> >
	class InCueMusicDecoderFactory : public ServiceFactory<IERServiceInCueMusicDecoderRegister>
	{
	public:
		InCueMusicDecoderFactory()
		{
			getPtr()->registInCueDecoder(&Allocator<Impl>::instance());
		}
	};

	template<typename T, typename Impl = MusicEncoderImpl<T> >
	class MusicEncoderFactory : public ServiceFactory<IERServiceMusicEncoderRegister>
	{
	public:
		MusicEncoderFactory()
		{
			getPtr()->registEncoder(&Allocator<Impl>::instance());
		}
	};
}

#endif
