#pragma once

#ifndef _INCLUDE_ERCOMPONENTMUSIC_
#define _INCLUDE_ERCOMPONENTMUSIC_

#include "IERComponentMusic.h"

namespace ERComponent
{
	template<typename T>
	class MusicDecoderImpl : public IERComponentMusicDecoder
	{
	};

	template<typename T>
	class IncueMusicDecoderImpl : public IERComponentIncueMusicDecoder
	{
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
			getPtr()->registerDecoder(&Allocator<Impl>::instance());
		}
	};

	template<typename T, typename Impl = IncueMusicDecoderImpl<T> >
	class IncueMusicDecoderFactory : public ServiceFactory<IERServiceIncueMusicDecoderRegister>
	{
	public:
		IncueMusicDecoderFactory()
		{
			getPtr()->registerIncueDecoder(&Allocator<Impl>::instance());
		}
	};

	template<typename T, typename Impl = MusicEncoderImpl<T> >
	class MusicEncoderFactory : public ServiceFactory<IERServiceMusicEncoderRegister>
	{
	public:
		MusicEncoderFactory()
		{
			getPtr()->registerEncoder(&Allocator<Impl>::instance());
		}
	};
}

#endif
