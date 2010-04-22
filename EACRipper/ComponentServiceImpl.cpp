#include "Defaults.h"

#include "MainController.h"
#include "MainWindow.h"
#include "ComponentServiceImpl.h"
#include "MusicCoderManager.h"

namespace EACRipper
{
	ERApplication::~ERApplication()
	{
	}

	void *ERApplication::getServicePointerImpl(const ERUUID &uuid, const void *param)
	{
		using namespace ServiceImpl;

		if(uuid == ERServiceUUID<IERServiceMusicDecoderRegister>::uuid)
			return &MusicDecoderRegister::instance();
		else if(uuid == ERServiceUUID<IERServiceInCueMusicDecoderRegister>::uuid)
			return &InCueMusicDecoderRegister::instance();
		else if(uuid == ERServiceUUID<IERServiceMusicEncoderRegister>::uuid)
			return &MusicEncoderRegister::instance();
		else if(uuid == ERServiceUUID<IERServiceStringCodepageConverter>::uuid)
			return &StringCodepageConverter::instance();

		return nullptr;
	}

	HWND ERApplication::getWindow() const
	{
		return MainWindow::instance().getWindow();
	}

	const IERComponentInfo *ERApplication::getInfo() const
	{
		return info;
	}

	void ERApplication::setInfo(IERComponentInfo *iinfo)
	{
		info = iinfo;
	}

	namespace ServiceImpl
	{
		MusicDecoderRegister::~MusicDecoderRegister()
		{
		}

		bool MusicDecoderRegister::registDecoder(IERAllocator *alloc)
		{
			IERComponentMusicDecoder *p = static_cast<IERComponentMusicDecoder *>(alloc->alloc());
			bool res = MusicCoderManager::instance().addCoder(p->getInfo().name, MusicCoderManager::Decoder, alloc);
			alloc->free(p);
			return res;
		}

		InCueMusicDecoderRegister::~InCueMusicDecoderRegister()
		{
		}

		bool InCueMusicDecoderRegister::registInCueDecoder(IERAllocator *alloc)
		{
			IERComponentInCueMusicDecoder *p = static_cast<IERComponentInCueMusicDecoder *>(alloc->alloc());
			bool res = MusicCoderManager::instance().addCoder(p->getInfo().name, MusicCoderManager::InCueDecoder, alloc);
			alloc->free(p);
			return res;
		}

		MusicEncoderRegister::~MusicEncoderRegister()
		{
		}

		bool MusicEncoderRegister::registEncoder(IERAllocator *alloc)
		{
			IERComponentMusicEncoder *p = static_cast<IERComponentMusicEncoder *>(alloc->alloc());
			bool res = MusicCoderManager::instance().addCoder(p->getInfo().name, MusicCoderManager::Encoder, alloc);
			alloc->free(p);
			return res;
		}

		StringCodepageConverter::StringCodepageConverter()
			: codepage(CP_ACP)
		{
		}

		StringCodepageConverter::~StringCodepageConverter()
		{
		}

		uint32_t StringCodepageConverter::getCodepage() const
		{
			return codepage;
		}

		bool StringCodepageConverter::setCodepage(uint32_t icodepage)
		{
			codepage = icodepage;
			return true;
		}

		size_t StringCodepageConverter::getConvertedLengthToUTF16(const char *str, size_t length)
		{
			return MultiByteToWideChar(codepage, 0, str, static_cast<int>(length), nullptr, 0);
		}

		size_t StringCodepageConverter::getConvertedLengthFromUTF16(const wchar_t *str, size_t length)
		{
			return WideCharToMultiByte(codepage, 0, str, static_cast<int>(length), nullptr, 0, nullptr, nullptr);
		}

		size_t StringCodepageConverter::convertToUTF16(wchar_t *toString, size_t toBufferLength, const char *fromString, size_t fromLength)
		{
			return MultiByteToWideChar(codepage, 0, fromString, static_cast<int>(fromLength), toString, static_cast<int>(toBufferLength));
		}

		size_t StringCodepageConverter::convertFromUTF16(char *toString, size_t toBufferLength, const wchar_t *fromString, size_t fromLength)
		{
			return WideCharToMultiByte(codepage, 0, fromString, static_cast<int>(fromLength), toString, static_cast<int>(toBufferLength), nullptr, nullptr);
		}
	}
}
