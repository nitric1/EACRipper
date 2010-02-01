#pragma once

class IERArchiveComponent;

#if defined(_WINDOWS) && defined(_USRDLL)
#include <windows.h>

extern "C" __declspec(dllexport) IERArchiveComponent *newInstance(HWND);
extern "C" __declspec(dllexport) void deleteInstance(IERArchiveComponent *);
#endif

#ifndef _INC_WINDOWS
#error Include <windows.h> before include this header!
#endif

#include <cinttypes>

class IERArchiveComponent
{
public:
	/**
	 * Main window's handle.
	 * If the component requires window handle, use this.
	 */
	HWND window;

public:
	/**
	 * Destructor for safe deriving.
	 */
	virtual __stdcall ~IERArchiveComponent() = 0 {}

	/**
	 * Open an archive file as read mode.
	 * @param name File name to be opened.
	 * @return true if the file is opened correctly, false otherwise.
	 */
	virtual bool __stdcall open(const wchar_t *name) = 0;

	/**
	 * Return file count of the opened file.
	 * @return File count.
	 */
	virtual uint32_t __stdcall getFileCount() const = 0;

	/**
	 * Return total size of files in the archive.
	 * @return Size in byte.
	 */
	virtual uint64_t __stdcall getTotalSize() const = 0;

	/**
	 * Extract the archive in rootPath.
	 * @param rootPath Path in which the archive is extracted.
	 * @return Total size of extracted data in byte if succeeded, std::numeric_limits<uint64_t>::max() in <limits> if failed.
	 */
	virtual uint64_t __stdcall extract(const wchar_t *rootPath) = 0;

	/**
	 * Extract the archive in rootPath by split.
	 * @param rootPath Path in which the archive is extracted.
	 * @param extractedSize Size of extracted data in byte.
	 * @param section The split section number; must be 0 if first-time read. Don't touch this if you want to extract continuously.
	 * @return true if succeeded and not final, false otherwise.
	 */
	virtual bool __stdcall extractSplit(const wchar_t *rootPath, uint64_t *extractedSize, uint64_t *section) = 0;

	/**
	 * Close the opened file.
	 */
	virtual void __stdcall close() = 0;

	/**
	 * Return information of the component.
	 * Should use ER_DECLARE_ARCHIVE_COMPONENT to override this function, instead of overriding directly.
	 */
	virtual const wchar_t *__stdcall getInformation() const = 0;
};

#define ER_ARCHIVE_COMPONENT_VERSION L"1.0.0"

#define ER_DECLARE_ARCHIVE_COMPONENT_DEFINITION() \
	public: virtual const wchar_t *getInformation() const;

#define ER_DECLARE_ARCHIVE_COMPONENT(name, ext, cls) \
	extern "C" IERArchiveComponent *newInstance(HWND window) { \
		IERArchiveComponent *p = new (cls)(); \
		p->window = window; \
		return p; \
	} \
	extern "C" void deleteInstance(IERArchiveComponent *p) { \
		delete p; \
	} \
	const wchar_t * ## cls ## ::getInformation() const { \
		return name L"\0" ext L"\0" ER_ARCHIVE_COMPONENT_VERSION L"\0"; \
	}
