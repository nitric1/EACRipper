#pragma once

#include "IERAllocator.h"
#include "IERStream.h"
#include "IERString.h"

#include "IERApplication.h"
#include "IERComponentEntrypoint.h"
#include "IERComponentInfo.h"

#include "IERComponentArchive.h"
#include "IERComponentMusic.h"

#include "Singleton.h"

namespace EACRipper
{
	class ERApplication : public IERApplication
	{
	private:
		IERComponentInfo *info;

	public:
		virtual ~ERApplication();

	private:
		virtual void *getServicePointerImpl(const ERUUID &uuid);

	public:
		virtual HWND getWindow() const;
		virtual const IERComponentInfo *getInfo() const;
		virtual void setInfo(IERComponentInfo *);

		friend class Singleton<ERApplication>;
	};

	class ERServiceMusicDecoderRegister : public IERServiceMusicDecoderRegister, public Singleton<ERServiceMusicDecoderRegister>
	{
	public:
		virtual ~ERServiceMusicDecoderRegister();

	public:
		virtual bool registDecoder(IERAllocator *);
	};

	class ERServiceInCueMusicDecoderRegister : public IERServiceInCueMusicDecoderRegister, public Singleton<ERServiceInCueMusicDecoderRegister>
	{
	public:
		virtual ~ERServiceInCueMusicDecoderRegister();

	public:
		virtual bool registInCueDecoder(IERAllocator *);
	};

	class ERServiceMusicEncoderRegister : public IERServiceMusicEncoderRegister, public Singleton<ERServiceMusicEncoderRegister>
	{
	public:
		virtual ~ERServiceMusicEncoderRegister();

	public:
		virtual bool registEncoder(IERAllocator *);
	};
}
