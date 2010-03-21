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

	void *ERApplication::getServicePointerImpl(const ERUUID &uuid)
	{
		if(uuid == ERServiceUUID<IERServiceMusicDecoderRegister>::uuid)
			return &ERServiceMusicDecoderRegister::instance();
		else if(uuid == ERServiceUUID<IERServiceInCueMusicDecoderRegister>::uuid)
			return &ERServiceInCueMusicDecoderRegister::instance();
		else if(uuid == ERServiceUUID<IERServiceMusicEncoderRegister>::uuid)
			return &ERServiceMusicEncoderRegister::instance();

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

	ERServiceMusicDecoderRegister::~ERServiceMusicDecoderRegister()
	{
	}

	bool ERServiceMusicDecoderRegister::registDecoder(IERAllocator *alloc)
	{
		IERComponentMusicDecoder *p = static_cast<IERComponentMusicDecoder *>(alloc->alloc());
		bool res = MusicCoderManager::instance().addCoder(p->getInfo().name, MusicCoderManager::Decoder, alloc);
		alloc->free(p);
		return res;
	}

	ERServiceInCueMusicDecoderRegister::~ERServiceInCueMusicDecoderRegister()
	{
	}

	bool ERServiceInCueMusicDecoderRegister::registInCueDecoder(IERAllocator *alloc)
	{
		IERComponentInCueMusicDecoder *p = static_cast<IERComponentInCueMusicDecoder *>(alloc->alloc());
		bool res = MusicCoderManager::instance().addCoder(p->getInfo().name, MusicCoderManager::InCueDecoder, alloc);
		alloc->free(p);
		return res;
	}

	ERServiceMusicEncoderRegister::~ERServiceMusicEncoderRegister()
	{
	}

	bool ERServiceMusicEncoderRegister::registEncoder(IERAllocator *alloc)
	{
		IERComponentMusicEncoder *p = static_cast<IERComponentMusicEncoder *>(alloc->alloc());
		bool res = MusicCoderManager::instance().addCoder(p->getInfo().name, MusicCoderManager::Encoder, alloc);
		alloc->free(p);
		return res;
	}
}
