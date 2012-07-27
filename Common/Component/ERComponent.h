#pragma once

#ifndef _INCLUDE_ERCOMPONENT_
#define _INCLUDE_ERCOMPONENT_

#include <cinttypes>

#include <limits>

#include <windows.h>

#include "ERTypes.h"
#include "EREndian.h"
#include "ERException.h"
#include "ERUUID.h"
#include "IERString.h"
#include "IERStream.h"
#include "IERFile.h"
#include "IERApplication.h"
#include "IERComponentArchive.h"
#include "IERComponentEntrypoint.h"
#include "IERComponentMusic.h"
#include "IERComponentInfo.h"

#if defined(_WINDOWS) && defined(_USRDLL)
extern "C" __declspec(dllexport) ERException *invokeCdecl(uintptr_t *, uintptr_t *, void *, uint32_t, ...);
extern "C" __declspec(dllexport) ERException *invokeThiscall(uintptr_t *, uintptr_t *, void *, void *, uint32_t, ...);
extern "C" __declspec(dllexport) void initComponent(IERApplication *);
extern "C" __declspec(dllexport) void uninitComponent();
#endif

#include "Component/Definition.h"
#include "Component/Invoke.h"
#include "Component/Info.h"
#include "Component/Factory.h"
#include "Component/Allocator.h"
#include "Component/Archive.h"
#include "Component/Music.h"

#define DECLARE_COMPONENT(name, version) \
	namespace ERComponentGlobal { \
		const wchar_t *componentName = name; \
		const wchar_t *componentVersion = version; \
		IERComponentEntrypoint *componentEntry = nullptr; \
		IERComponentInfo *info = nullptr; \
	}

#if defined(_WINDOWS) && defined(_USRDLL)
extern "C" __declspec(dllexport) ERException *invokeCdecl(uintptr_t *retValEax, uintptr_t *retValEdx, void *fn, uint32_t argc, ...)
{
#ifndef _WIN64 // x86
	static const uint8_t invokeCode[] =
	{
		0x55, // PUSH EBP
		0x89, 0xE5, // MOV EBP, ESP
		0x8B, 0x45, 0x10, // MOV EAX, DWORD PTR [EBP+16]
		0x8B, 0x10, // MOV EDX, DWORD PTR [EAX]
		0x52, // PUSH EDX
		0x05, 0x04, 0x00, 0x00, 0x00, // ADD EAX, 4
		0x8B, 0x08, // MOV ECX, [EAX]
		0x8D, 0x04, 0x88, // LEA EAX, [EAX+ECX*4]
		0x8B, 0x10, // ArgLoop: MOV EDX, DWORD PTR [EAX]
		0x52, // PUSH EDX
		0x2D, 0x04, 0x00, 0x00, 0x00, // SUB EAX, 4
		0xE2, 0xF6, // LOOP ArgLoop
		0x8B, 0x55, 0xFC, // MOV EDX, [EBP-4]
		0xFF, 0xD2, // CALL EDX
		0x8B, 0x4D, 0x0C, // MOV ECX, [EBP+12]
		0x89, 0x11, // MOV [ECX], EDX
		0x8B, 0x4D, 0x08, // MOV ECX, [EBP+8]
		0x89, 0x01, // MOV [ECX], EAX
		0x89, 0xEC, // MOV ESP, EBP
		0x5D, // POP EBP
		0xC3, // RET
		0xCC, // INT 3
	};
#else // x64
	static const uint8_t invokeCode[] =
	{
		0x41, 0x54, // PUSH R12
		0x41, 0x55, // PUSH R13
		0x41, 0x56, // PUSH R14
		0x41, 0x57, // PUSH R15
		0x56, // PUSH RSI
		0x57, // PUSH RDI
		0x48, 0x83, 0xEC, 0x68, // SUB RSP, 104
		0x48, 0xC7, 0x02, 0x00, 0x00, 0x00, 0x00, // MOV QWORD PTR [RDX], 0
		0x49, 0x8B, 0xF0, // MOV RSI, R8
		0x4C, 0x8B, 0x26, // MOV R12, [RSI]
		0x4C, 0x8B, 0xE9, // MOV R13, RCX
		0x48, 0x83, 0xC6, 0x08, // ADD RSI, 8
		0x48, 0x33, 0xC0, // XOR RAX, RAX
		0x8B, 0x06, // MOV EAX, DWORD PTR [RSI]
		0x4C, 0x8B, 0xF0, // MOV R14, RAX
		0x49, 0x83, 0xFE, 0x0D, // CMP R14, 13
		0x77, 0x60, // JA Finish
		0x4E, 0x8D, 0x3C, 0xF5, 0x00, 0x00, 0x00, 0x00, // LEA R15, [R14*8]
		0x49, 0x83, 0xFE, 0x00, // CMP R14, 0
		0x74, 0x52, // JZ Calling
		0x48, 0x83, 0xC6, 0x08, // ADD RSI, 8
		0x48, 0x8B, 0x0E, // MOV RCX, [RSI]
		0x49, 0xFF, 0xCE, // DEC R14
		0x74, 0x46, // JZ Calling
		0x48, 0x83, 0xC6, 0x08, // ADD RSI, 8
		0x48, 0x8B, 0x16, // MOV RDX, [RSI]
		0x49, 0xFF, 0xCE, // DEC R14
		0x74, 0x3A, // JZ Calling
		0x48, 0x83, 0xC6, 0x08, // ADD RSI, 8
		0x4C, 0x8B, 0x06, // MOV R8, [RSI]
		0x49, 0xFF, 0xCE, // DEC R14
		0x74, 0x2E, // JZ Calling
		0x48, 0x83, 0xC6, 0x08, // ADD RSI, 8
		0x4C, 0x8B, 0x0E, // MOV R9, [RSI]
		0x49, 0xFF, 0xCE, // DEC R14
		0x74, 0x22, // JZ Calling
		0x4A, 0x8D, 0x34, 0xF6, // LEA RSI, [RSI+R14*8]
		0x4A, 0x8D, 0x3C, 0x3C, // LEA RDI, [RSP+R15]
		0x48, 0x83, 0xEF, 0x08, // ArgLoop: SUB RDI, 8
		0x48, 0x8B, 0x06, // MOV RAX, [RSI]
		0x48, 0x89, 0x07, // MOV [RDI], RAX
		0x48, 0x83, 0xEE, 0x08, // SUB RSI, 8
		0x49, 0xFF, 0xCE, // DEC R14
		0x75, 0xED, // JNZ Calling
		0x41, 0xFF, 0xD4, // Calling: CALL R12
		0x49, 0x89, 0x45, 0x00, // MOV [R13], RAX
		0x48, 0x83, 0xC4, 0x68, // Finish: ADD RSP, 104
		0x5F, // POP RDI
		0x5E, // POP RSI
		0x41, 0x5F, // POP R15
		0x41, 0x5E, // POP R14
		0x41, 0x5D, // POP R13
		0x41, 0x5C, // POP R12
		0xC3, // RET
		0xCC, // INT 3
	};
#endif

	void *invokeImplStorage = VirtualAlloc(nullptr, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if(invokeImplStorage == nullptr)
	{
		// TODO: return exception
	}

	memcpy(invokeImplStorage, invokeCode, sizeof(invokeCode));

#ifdef _WIN64
	uintptr_t rfAddress = reinterpret_cast<uintptr_t>(invokeImplStorage) + sizeof(invokeCode) + 10;
	rfAddress += 16 - (rfAddress % 16);
	assert((rfAddress % 16) == 0);
	RUNTIME_FUNCTION *rf = reinterpret_cast<RUNTIME_FUNCTION *>(rfAddress);
	uint8_t *unwind = reinterpret_cast<uint8_t *>(rfAddress + 16);

	rf->BeginAddress = 0;
	rf->EndAddress = sizeof(invokeCode);
	rf->UnwindData = static_cast<uint32_t>(reinterpret_cast<uint8_t *>(unwind) - static_cast<uint8_t *>(invokeImplStorage));

	unwind[0] = 1; // Version | (Flags << 3)
	unwind[1] = 14; // 18; // SizeOfProlog
	unwind[2] = 7; // 9; // CountOfCodes
	unwind[3] = 0; // 149; // FrameRegister | (FrameOffset << 4)
	// UNWIND_CODE:
	unwind[4] = 18;
	unwind[5] = 194; // UWOP_ALLOC_SMALL | (12 << 4)
	unwind[6] = 14;
	unwind[7] = 112; // UWOP_PUSH_NONVOL | (7 << 4)
	unwind[8] = 13;
	unwind[9] = 96; // UWOP_PUSH_NONVOL | (6 << 4)
	unwind[10] = 12;
	unwind[11] = 240; // UWOP_PUSH_NONVOL | (15 << 4)
	unwind[12] = 10;
	unwind[13] = 224; // UWOP_PUSH_NONVOL | (14 << 4)
	unwind[14] = 8;
	unwind[15] = 208; // UWOP_PUSH_NONVOL | (13 << 4)
	unwind[16] = 6;
	unwind[17] = 192; // UWOP_PUSH_NONVOL | (12 << 4)

	BOOL res = RtlAddFunctionTable(rf, 1, reinterpret_cast<uintptr_t>(invokeImplStorage));
	assert(res);
#endif

	ERException *retExcp = nullptr;

	try
	{
		uintptr_t retValEaxTmp, retValEdxTmp;
		typedef void (*ImplFn)(uintptr_t *, uintptr_t *, void **);
		ImplFn invokeImpl = reinterpret_cast<ImplFn>(invokeImplStorage);

		invokeImpl(&retValEaxTmp, &retValEdxTmp, &fn);

		if(retValEax != nullptr)
			*retValEax = retValEaxTmp;
		if(retValEdx != nullptr)
			*retValEdx = retValEdxTmp;
	}
	catch(ERException *e)
	{
		retExcp = e;
	}
	catch(std::exception &e)
	{
		// TODO: std::exception to ERException
		retExcp = nullptr;
	}
	catch(...)
	{
		// TODO: Unknown exception
		retExcp = nullptr;
	}

#ifdef _WIN64
	RtlDeleteFunctionTable(rf);
#endif

	VirtualFree(invokeImplStorage, 0, MEM_RELEASE);

	return retExcp;
}

/*
void fn(int a, int b, int c, int d, int e, int f, int g)
{
	if(g == 0)
	{
		throw(0);
	}

	fn(a, b, c, d, e, f, g - 1);
}

extern "C" __declspec(dllexport) void occur()
{
	invokeCdecl(nullptr, nullptr, fn, 7, 8, 7, 6, 5, 4, 3, 2);
}*/

extern "C" __declspec(dllexport) ERException *invokeThiscall(uintptr_t *retValEax, uintptr_t *retValEdx, void *fn, void *instance, uint32_t argc, ...)
{
#ifndef _WIN64 // x86
	static const uint8_t invokeCode[] =
	{
		0x55, // PUSH EBP
		0x89, 0xE5, // MOV EBP, ESP
		0x8B, 0x45, 0x10, // MOV EAX, DWORD PTR [EBP+16]
		0x8B, 0x10, // MOV EDX, DWORD PTR [EAX]
		0x52, // PUSH EDX
		0x05, 0x04, 0x00, 0x00, 0x00, // ADD EAX, 4
		0x8B, 0x08, // MOV ECX, [EAX]
		0x8D, 0x04, 0x88, // LEA EAX, [EAX+ECX*4]
		0x8B, 0x10, // ArgLoop: MOV EDX, DWORD PTR [EAX]
		0x52, // PUSH EDX
		0x2D, 0x04, 0x00, 0x00, 0x00, // SUB EAX, 4
		0xE2, 0xF6, // LOOP ArgLoop
		0x8B, 0x55, 0xFC, // MOV EDX, [EBP-4]
		0x8B, 0x4D, 0x14, // MOV ECX, [EBP+20]
		0xFF, 0xD2, // CALL EDX
		0x8B, 0x4D, 0x0C, // MOV ECX, [EBP+12]
		0x89, 0x11, // MOV [ECX], EDX
		0x8B, 0x4D, 0x08, // MOV ECX, [EBP+8]
		0x89, 0x01, // MOV [ECX], EAX
		0x89, 0xEC, // MOV ESP, EBP
		0x5D, // POP EBP
		0xC3, // RET
	};
#else // x64
	static const uint8_t invokeCode[] =
	{
		0x41, 0x54, // PUSH R12
		0x41, 0x55, // PUSH R13
		0x41, 0x56, // PUSH R14
		0x41, 0x57, // PUSH R15
		0x56, // PUSH RSI
		0x57, // PUSH RDI
		0x48, 0x83, 0xEC, 0x68, // SUB RSP, 104
		0x48, 0xC7, 0x02, 0x00, 0x00, 0x00, 0x00, // MOV QWORD PTR [RDX], 0
		0x49, 0x8B, 0xF0, // MOV RSI, R8
		0x4C, 0x8B, 0x26, // MOV R12, [RSI]
		0x4C, 0x8B, 0xE9, // MOV R13, RCX
		0x48, 0x83, 0xC6, 0x08, // ADD RSI, 8
		0x48, 0x33, 0xC0, // XOR RAX, RAX
		0x8B, 0x06, // MOV EAX, DWORD PTR [RSI]
		0x4C, 0x8B, 0xF0, // MOV R14, RAX
		0x49, 0x83, 0xFE, 0x0C, // CMP R14, 12
		0x77, 0x60, // JA Finish
		0x4E, 0x8D, 0x3C, 0xF5, 0x00, 0x00, 0x00, 0x00, // LEA R15, [R14*8]
		0x49, 0x8B, 0xC9, // MOV RCX, R9
		0x49, 0x83, 0xFE, 0x00, // CMP R14, 0
		0x74, 0x52, // JZ Calling
		0x48, 0x83, 0xC6, 0x08, // ADD RSI, 8
		0x48, 0x8B, 0x16, // MOV RDX, [RSI]
		0x49, 0xFF, 0xCE, // DEC R14
		0x74, 0x3A, // JZ Calling
		0x48, 0x83, 0xC6, 0x08, // ADD RSI, 8
		0x4C, 0x8B, 0x06, // MOV R8, [RSI]
		0x49, 0xFF, 0xCE, // DEC R14
		0x74, 0x2E, // JZ Calling
		0x48, 0x83, 0xC6, 0x08, // ADD RSI, 8
		0x4C, 0x8B, 0x0E, // MOV R9, [RSI]
		0x49, 0xFF, 0xCE, // DEC R14
		0x74, 0x22, // JZ Calling
		0x4A, 0x8D, 0x34, 0xF6, // LEA RSI, [RSI+R14*8]
		0x4A, 0x8D, 0x3C, 0x3C, // LEA RDI, [RSP+R15]
		0x48, 0x83, 0xEF, 0x08, // ArgLoop: SUB RDI, 8
		0x48, 0x8B, 0x06, // MOV RAX, [RSI]
		0x48, 0x89, 0x07, // MOV [RDI], RAX
		0x48, 0x83, 0xEE, 0x08, // SUB RSI, 8
		0x49, 0xFF, 0xCE, // DEC R14
		0x75, 0xED, // JNZ Calling
		0x41, 0xFF, 0xD4, // Calling: CALL R12
		0x49, 0x89, 0x45, 0x00, // MOV [R13], RAX
		0x48, 0x83, 0xC4, 0x68, // Finish: ADD RSP, 104
		0x5F, // POP RDI
		0x5E, // POP RSI
		0x41, 0x5F, // POP R15
		0x41, 0x5E, // POP R14
		0x41, 0x5D, // POP R13
		0x41, 0x5C, // POP R12
		0xC3, // RET
		0xCC, // INT 3
	};
#endif

	void *invokeImplStorage = VirtualAlloc(nullptr, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if(invokeImplStorage == nullptr)
	{
		// TODO: return exception
	}

	memcpy(invokeImplStorage, invokeCode, sizeof(invokeCode));

#ifdef _WIN64
	uintptr_t rfAddress = reinterpret_cast<uintptr_t>(invokeImplStorage) + sizeof(invokeCode) + 10;
	rfAddress += 16 - (rfAddress % 16);
	assert((rfAddress % 16) == 0);
	RUNTIME_FUNCTION *rf = reinterpret_cast<RUNTIME_FUNCTION *>(rfAddress);
	uint8_t *unwind = reinterpret_cast<uint8_t *>(rfAddress + 16);

	rf->BeginAddress = 0;
	rf->EndAddress = sizeof(invokeCode);
	rf->UnwindData = static_cast<uint32_t>(reinterpret_cast<uint8_t *>(unwind) - static_cast<uint8_t *>(invokeImplStorage));

	unwind[0] = 1; // Version | (Flags << 3)
	unwind[1] = 14; // 18; // SizeOfProlog
	unwind[2] = 7; // 9; // CountOfCodes
	unwind[3] = 0; // 149; // FrameRegister | (FrameOffset << 4)
	// UNWIND_CODE:
	unwind[4] = 18;
	unwind[5] = 194; // UWOP_ALLOC_SMALL | (12 << 4)
	unwind[6] = 14;
	unwind[7] = 112; // UWOP_PUSH_NONVOL | (7 << 4)
	unwind[8] = 13;
	unwind[9] = 96; // UWOP_PUSH_NONVOL | (6 << 4)
	unwind[10] = 12;
	unwind[11] = 240; // UWOP_PUSH_NONVOL | (15 << 4)
	unwind[12] = 10;
	unwind[13] = 224; // UWOP_PUSH_NONVOL | (14 << 4)
	unwind[14] = 8;
	unwind[15] = 208; // UWOP_PUSH_NONVOL | (13 << 4)
	unwind[16] = 6;
	unwind[17] = 192; // UWOP_PUSH_NONVOL | (12 << 4)

	BOOL res = RtlAddFunctionTable(rf, 1, reinterpret_cast<uintptr_t>(invokeImplStorage));
	assert(res);
#endif

	ERException *retExcp = nullptr;

	try
	{
		uintptr_t retValEaxTmp, retValEdxTmp;
		typedef void (*ImplFn)(uintptr_t *, uintptr_t *, void **, void *);
		ImplFn invokeImpl = reinterpret_cast<ImplFn>(invokeImplStorage);

		invokeImpl(&retValEaxTmp, &retValEdxTmp, &fn, instance);

		if(retValEax != nullptr)
			*retValEax = retValEaxTmp;
		if(retValEdx != nullptr)
			*retValEdx = retValEdxTmp;
	}
	catch(ERException *e)
	{
		retExcp = e;
	}
	catch(std::exception &e)
	{
		// TODO: std::exception to ERException
		retExcp = nullptr;
	}
	catch(...)
	{
		// TODO: Unknown exception
		retExcp = nullptr;
	}

#ifdef _WIN64
	RtlDeleteFunctionTable(rf);
#endif

	VirtualFree(invokeImplStorage, 0, MEM_RELEASE);

	return nullptr;
}

extern "C" __declspec(dllexport) void initComponent(IERApplication *app)
{
	ERComponent::ApplicationInfo::instance().setApp(app);
	ERComponentGlobal::info = new ERComponent::Info();
	app->setInfo(ERComponentGlobal::info);
	if(ERComponentGlobal::componentEntry != nullptr)
		ERComponentGlobal::componentEntry->onInit();
}

extern "C" __declspec(dllexport) void uninitComponent()
{
	if(ERComponentGlobal::componentEntry != nullptr)
		ERComponentGlobal::componentEntry->onUninit();
	delete ERComponentGlobal::info;
}
#endif

#endif
