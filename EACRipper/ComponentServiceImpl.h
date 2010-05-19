#pragma once

#include "Component/Definition.h"

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
	class ERApplication;

	class ERServicePointer
	{
	public:
		virtual ~ERServicePointer();

	public:
		virtual void *get() = 0;
	};

	template<typename T>
	class ERServicePointerImpl : public ERServicePointer
	{
	private:
		std::tr1::shared_ptr<T> ptr;

	public:
		ERServicePointerImpl(T *iptr) : ptr(iptr) {}
		template<typename Other>
		ERServicePointerImpl(const std::tr1::shared_ptr<Other> &iptr) : ptr(iptr) {}
		virtual ~ERServicePointerImpl() {}

	public:
		virtual void *get() { return ptr.get(); }
	};

	class ERApplication : public IERApplication
	{
	private:
		IERComponentInfo *info;
		std::list<std::tr1::shared_ptr<ERServicePointer>> ptrPool;

	public:
		virtual ~ERApplication();

	private:
		template<typename T>
		T *appendPointer(T *ptr)
		{
			ERServicePointerImpl<T> *sptr = new ERServicePointerImpl<T>(ptr);
			ptrPool.push_back(shared_ptr<ERServicePointer>(sptr));
			return ptr;
		}
		void removePointer(void *);

	private:
		virtual void *getServicePointerImpl(const ERUUID &uuid, const void *param);
		virtual void removeServicePointerImpl(const ERUUID &uuid, void *ptr);

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

		class StringCodepageConverter : public IERServiceStringCodepageConverter
		{
		private:
			uint32_t codepage;

		public:
			StringCodepageConverter();
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
