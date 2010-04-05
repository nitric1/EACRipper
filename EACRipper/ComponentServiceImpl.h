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
		virtual void *getServicePointerImpl(const ERUUID &uuid, const void *param);

	public:
		virtual HWND getWindow() const;
		virtual const IERComponentInfo *getInfo() const;
		virtual void setInfo(IERComponentInfo *);

		// friend class Singleton<ERApplication>;
	};

	namespace ServiceImpl
	{
		class MusicDecoderRegister : public IERServiceMusicDecoderRegister, public Singleton<MusicDecoderRegister>
		{
		private:
			virtual ~MusicDecoderRegister();

		public:
			virtual bool registDecoder(IERAllocator *);

			friend class Singleton<MusicDecoderRegister>;
		};

		class InCueMusicDecoderRegister : public IERServiceInCueMusicDecoderRegister, public Singleton<InCueMusicDecoderRegister>
		{
		private:
			virtual ~InCueMusicDecoderRegister();

		public:
			virtual bool registInCueDecoder(IERAllocator *);

			friend class Singleton<InCueMusicDecoderRegister>;
		};

		class MusicEncoderRegister : public IERServiceMusicEncoderRegister, public Singleton<MusicEncoderRegister>
		{
		private:
			virtual ~MusicEncoderRegister();

		public:
			virtual bool registEncoder(IERAllocator *);

			friend class Singleton<MusicEncoderRegister>;
		};

		class StringCodepageConverter : public IERServiceStringCodepageConverter, public Singleton<StringCodepageConverter>
		{
		private:
			uint32_t codepage;

		private:
			virtual ~StringCodepageConverter();

		public:
			virtual uint32_t getCodepage() const;
			virtual bool setCodepage(uint32_t);

			virtual size_t getConvertedLengthToUTF16(const char *, size_t = std::numeric_limits<size_t>::max());
			virtual size_t getConvertedLengthFromUTF16(const wchar_t *, size_t = std::numeric_limits<size_t>::max());
			virtual size_t convertToUTF16(wchar_t *, size_t, const char *, size_t = std::numeric_limits<size_t>::max());
			virtual size_t convertFromUTF16(char *, size_t, const wchar_t *, size_t = std::numeric_limits<size_t>::max());

			friend class Singleton<StringCodepageConverter>;
		};
	}
}
