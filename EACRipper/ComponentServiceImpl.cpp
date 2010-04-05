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

		return NULL;
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
	}
}
