#pragma once

#include "Component/Definition.h"

#include "IERAllocator.h"
#include "IERStream.h"
#include "IERFile.h"
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

	class ServicePointer
	{
	public:
		virtual ~ServicePointer();

	public:
		virtual void *get() = 0;
	};

	template<typename T>
	class ServicePointerImpl : public ServicePointer
	{
	private:
		std::tr1::shared_ptr<T> ptr;

	public:
		ServicePointerImpl(T *iptr) : ptr(iptr) {}
		template<typename Other>
		ServicePointerImpl(const std::tr1::shared_ptr<Other> &iptr) : ptr(iptr) {}
		virtual ~ServicePointerImpl() {}

	public:
		virtual void *get() { return ptr.get(); }
	};

	class ServicePointerManager : public Singleton<ServicePointerManager>
	{
	private:
		std::list<std::tr1::shared_ptr<ServicePointer>> ptrPool;

	private:
		virtual ~ServicePointerManager();

	public:
		template<typename T>
		T *append(T *ptr)
		{
			ServicePointerImpl<T> *sptr = new ServicePointerImpl<T>(ptr);
			ptrPool.push_back(std::tr1::shared_ptr<ServicePointer>(sptr));
			return ptr;
		}
		void remove(void *);

		friend class Singleton<ServicePointerManager>;
	};

	class ERApplication : public IERApplication
	{
	private:
		IERComponentInfo *info;

	public:
		virtual ~ERApplication();

	private:
		template<typename T>
		T *appendPointer(T *ptr)
		{
			return ServicePointerManager::instance().append(ptr);
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
		};

		class StringCharsetConverter : public IERServiceStringCharsetConverter
		{
		private:
			std::string charset;
			UConverter *cv;

		public:
			StringCharsetConverter();
			virtual ~StringCharsetConverter();

		private:
			void makeConverter();

		public:
			virtual const char *getCharset() const;
			virtual bool setCharset(const char *);

			virtual size_t getConvertedLengthToUTF16(const char *, size_t = std::numeric_limits<size_t>::max());
			virtual size_t getConvertedLengthFromUTF16(const wchar_t *, size_t = std::numeric_limits<size_t>::max());
			virtual size_t convertToUTF16(wchar_t *, size_t, const char *, size_t = std::numeric_limits<size_t>::max());
			virtual size_t convertFromUTF16(char *, size_t, const wchar_t *, size_t = std::numeric_limits<size_t>::max());
		};

		class CharsetDetector : public IERServiceCharsetDetector
		{
		public:
			virtual ~CharsetDetector();

		public:
			virtual IERServiceStringConverter *detect(const char *);
		};

		class LocalFile : public IERLocalFile
		{
		private:
			uint32_t attr;
			std::wstring path;

		public:
			LocalFile();
			virtual ~LocalFile();

		public:
			virtual bool open(const wchar_t *);
			virtual bool close();

			virtual bool exists();
			virtual bool canRead();
			virtual bool canWrite();

			virtual IERStreamReader *getStreamReader(bool = false);
			virtual IERStreamWriter *getStreamWriter(bool = true);
		};

		class LocalDirectory : public IERLocalDirectory
		{
		private:
			std::wstring path;

		public:
			LocalDirectory();
			virtual ~LocalDirectory();

		public:
			virtual bool open(const wchar_t *, bool = false);
			virtual bool close();

			virtual IERFile *getFile(const wchar_t *);
		};
	}
}
