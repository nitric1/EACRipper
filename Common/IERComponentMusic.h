#pragma once

#include "ERUUID.h"

#include "IERAllocator.h"

class IERComponentMusicDecoder
{
public:
	virtual ~IERComponentMusicDecoder() = 0 {}
};

class IERComponentIncueMusicDecoder : public IERComponentMusicDecoder
{
public:
	virtual ~IERComponentIncueMusicDecoder() = 0 {}
};

class IERComponentMusicEncoder
{
public:
	virtual ~IERComponentMusicEncoder() = 0 {}
};

class IERServiceMusicDecoderRegister
{
public:
	virtual ~IERServiceMusicDecoderRegister() = 0 {}

public:
	virtual bool registerDecoder(IERAllocator *) = 0;
};

class IERServiceIncueMusicDecoderRegister
{
public:
	virtual ~IERServiceIncueMusicDecoderRegister() = 0 {}

public:
	virtual bool registerIncueDecoder(IERAllocator *) = 0;
};

class IERServiceMusicEncoderRegister
{
public:
	virtual ~IERServiceMusicEncoderRegister() = 0 {}

public:
	virtual bool registerEncoder(IERAllocator *) = 0;
};

template<>
const ERUUID ERServiceUUID<IERServiceMusicDecoderRegister>::uuid
	= ERUUID(0x25C12300, 0xE3D9, 0x4C1F, 0x8496, 0xF8, 0xF1, 0x22, 0x30, 0x5A, 0x6C);
// 25C12300-E3D9-4C1F-8496-F8F122305A6C

template<>
const ERUUID ERServiceUUID<IERServiceIncueMusicDecoderRegister>::uuid
	= ERUUID(0xD13C888F, 0xB4F2, 0x46AA, 0xA7CE, 0x85, 0x66, 0x42, 0x5E, 0xE6, 0x35);
// D13C888F-B4F2-46AA-A7CE-8566425EE635

template<>
const ERUUID ERServiceUUID<IERServiceMusicEncoderRegister>::uuid
	= ERUUID(0xD15C7DA9, 0x2E12, 0x4552, 0xB401, 0x5A, 0x2A, 0x3A, 0x40, 0xB8, 0x21);
// D15C7DA9-2E12-4552-B401-5A2A3A40B821
