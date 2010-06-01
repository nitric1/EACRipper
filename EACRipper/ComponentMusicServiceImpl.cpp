#include "Defaults.h"

#include "ComponentServiceImpl.h"
#include "CoderManager.h"

using namespace std;

namespace EACRipper
{
	namespace ServiceImpl
	{
		MusicDecoderRegister::~MusicDecoderRegister()
		{
		}

		bool MusicDecoderRegister::registDecoder(IERAllocator *alloc)
		{
			IERComponentMusicDecoder *p = static_cast<IERComponentMusicDecoder *>(alloc->alloc());
			bool res = MusicCoderManager::instance().addCoder(make_pair(p->getInfo().name, MusicCoderManager::Decoder), alloc);
			alloc->free(p);
			return res;
		}

		InCueMusicDecoderRegister::~InCueMusicDecoderRegister()
		{
		}

		bool InCueMusicDecoderRegister::registInCueDecoder(IERAllocator *alloc)
		{
			IERComponentInCueMusicDecoder *p = static_cast<IERComponentInCueMusicDecoder *>(alloc->alloc());
			bool res = MusicCoderManager::instance().addCoder(make_pair(p->getInfo().name, MusicCoderManager::InCueDecoder), alloc);
			alloc->free(p);
			return res;
		}

		MusicEncoderRegister::~MusicEncoderRegister()
		{
		}

		bool MusicEncoderRegister::registEncoder(IERAllocator *alloc)
		{
			IERComponentMusicEncoder *p = static_cast<IERComponentMusicEncoder *>(alloc->alloc());
			bool res = MusicCoderManager::instance().addCoder(make_pair(p->getInfo().name, MusicCoderManager::Encoder), alloc);
			alloc->free(p);
			return res;
		}
	}
}
