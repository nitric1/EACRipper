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

	template<typename T, typename Impl = MusicDecoderImpl<T> >
	class MusicDecoderRegister
	{
	};

	template<typename T>
	class MusicEncoderImpl : public IERComponentMusicEncoder
	{
	};

	template<typename T, typename Impl = MusicEncoderImpl<T> >
	class MusicEncoderRegister
	{
	};
}

#endif
