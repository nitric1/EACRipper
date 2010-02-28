#pragma once

class IERMusicComponent;

#if defined(_WINDOWS) && defined(_USRDLL)
#include <windows.h>

extern "C" __declspec(dllexport) IERMusicComponent *newInstance(HWND);
extern "C" __declspec(dllexport) void deleteInstance(IERMusicComponent *);
#endif

#ifndef _INC_WINDOWS
#error Include <windows.h> before include this header!
#endif

#include <cinttypes>

class IERMusicComponent
{
public:
	/**
	 * Struct of format list.
	 */
	typedef struct _Format
	{
		uint32_t number;
		const wchar_t *name;
	} Format;

private:
	/**
	 * Main window's handle.
	 * If the component requires window handle (like DirectSound), use this.
	 */
	HWND window;

public:
	/**
	 * Destructor for safe deriving.
	 */
	virtual __stdcall ~IERMusicComponent() = 0 {}

	/**
	 * Return this class' owner window.
	 * @return Window handle of owner.
	 */
	virtual HWND __stdcall getWindow()
	{
		return window;
	}

	/**
	 * Set this class' owner window.
	 * @param windowArg Window handle of new owner.
	 */
	virtual void __stdcall setWindow(HWND windowArg)
	{
		window = windowArg;
	}

	/**
	 * Open a file as read mode.
	 * @param name File name to be opened.
	 * @return true if the file is opened correctly, false otherwise.
	 */
	virtual bool __stdcall openRead(const wchar_t *name) = 0;

	/**
	 * Return channels of the opened file.
	 * @return The number of channels.
	 */
	virtual uint8_t __stdcall getChannels() const = 0;

	/**
	 * Return bits per sample of the opened file.
	 * @return The number of bits per sample.
	 */
	virtual uint8_t __stdcall getBitsPerSample() const = 0;

	/**
	 * Return sampling rate of the opened file.
	 * @return Sampling rate value in Hz.
	 */
	virtual uint32_t __stdcall getSamplingRate() const = 0;

	/**
	 * Return music length of the opened file.
	 * @return Length in millisecond.
	 */
	virtual uint32_t __stdcall getLength() const = 0;

	/**
	 * Return required buffer size in the section [startMillisec, endMillisec).
	 * @param startMillisec Start of the section in millisecond.
	 * @param endMillisec End of the section in millisecond.
	 * @return Required buffer size in byte.
	 */
	virtual uint32_t __stdcall getSize(uint32_t startMillisec, uint32_t endMillisec) const = 0
	{
		return static_cast<uint32_t>(getChannels()) * static_cast<uint32_t>(getBitsPerSample() / 8) * static_cast<uint32_t>(static_cast<uint64_t>(endMillisec - startMillisec) * getSamplingRate() / 1000);
	}

	/**
	 * Return required buffer array length of inCue string.
	 * @param codePage Default code page for inCue encoding.
	 * @return Required buffer array length.
	 */
	virtual size_t __stdcall getInCueLength(uint32_t codePage) = 0;

	/**
	 * Read inCue string in the opened file.
	 * @param buffer Buffer in which inCue string is to be stored.
	 * @param bufferSize The buffer's array length.
	 * @param codePage Default code page for inCue encoding.
	 * @return true if succeeded, false otherwise.
	 */
	virtual bool __stdcall readInCue(wchar_t *buffer, size_t bufferSize, uint32_t codePage) = 0;

	/**
	 * Read music data of the opened file from the section [startMillisec, endMillisec).
	 * @param startMillisec Start of the section in millisecond.
	 * @param endMillisec End of the section in millisecond.
	 * @param buffer Buffer in which music data is to be stored.
	 * @param bufferSize The buffer's size in byte.
	 * @return Size of read data in byte if succeeded, std::numeric_limits<unsigned>::max() in <limits> if failed.
	 */
	virtual size_t __stdcall read(uint32_t startMillisec, uint32_t endMillisec, void *buffer, size_t bufferSize) = 0;

	/**
	 * Return required buffer size to read by split.
	 * @return Required buffer size in byte.
	 */
	virtual size_t __stdcall getSplitSize() const = 0;

	/**
	 * Read the opened file from the section [startMillisec, endMillisec) by split.
	 * @param startMillisec Start of the section in millisecond.
	 * @param endMillisec End of the section in millisecond.
	 * @param buffer Buffer in which data is to be stored.
	 * @param bufferSize The buffer's size in byte.
	 * @param readSize Size of read data in byte.
	 * @param section The split section number; must be 0 if first-time read. Don't touch this if you want to read continuously.
	 * @return true if succeeded and not final, false otherwise.
	 */
	virtual bool __stdcall readSplit(uint32_t startMillisec, uint32_t endMillisec, void *buffer, size_t bufferSize, size_t *readSize, uint64_t *section) = 0;

	/**
	 * Open a file as write mode.
	 * @param name File name to be opened.
	 * @param channels Channels of the saved file.
	 * @param bitsPerSample Bits per sample of the saved file.
	 * @param samplingRate Sampling rate of the saved file.
	 * @param totalSize Total data size to be saved.
	 * @param format One of formats listed in ER_DECLARE_COMPONENT_FORMAT_*().
	 * @return true if the file is opened correctly, false otherwise.
	 */
	virtual bool __stdcall openWrite(const wchar_t *name, uint8_t channels, uint8_t bitsPerSample, uint32_t samplingRate, uint32_t totalSize, uint32_t format) = 0;

	/**
	 * Set a music tag in the opened file.
	 * @param field Tag's name.
	 * @param value Tag's value.
	 */
	virtual void __stdcall setTag(const wchar_t *field, const wchar_t *value) = 0;

	/**
	 * Set cover art in the opened file.
	 * @param path Cover art's file path.
	 * @return true if the cover art is correctly set, false otherwise.
	 */
	virtual bool __stdcall setCoverArt(const wchar_t *name) = 0;

	/**
	 * Write the data in the opened file.
	 * @param buffer Buffer of data to be saved.
	 * @param bufferSize The buffer's size in byte.
	 * @return true if the data is saved correctly, false otherwise.
	 */
	virtual bool __stdcall write(const void *buffer, size_t bufferSize) = 0;

	/**
	 * Close the opened file.
	 */
	virtual void __stdcall close() = 0;

	/**
	 * Return information of the component.
	 * Should use ER_DECLARE_MUSIC_COMPONENT_DEFINITION and ER_DECLARE_MUSIC_COMPONENT macros to override this function, instead of overriding directly.
	 */
	virtual const wchar_t *__stdcall getInformation() const = 0;

	/**
	 * Return format list.
	 * Should use ER_DECLARE_MUSIC_COMPONENT_FORMAT_* macros to override this function, instead of overriding directly.
	 */
	virtual const Format *__stdcall getFormats() const = 0;

	/**
	 * Return whether the component is compiled as debug mode or not.
	 * Should use ER_DECLARE_MUSIC_COMPONENT_DEFINITION and ER_DECLARE_MUSIC_COMPONENT macros to override this function, instead of overriding directly.
	 */
	virtual bool __stdcall isDebug() const = 0;
};

#define ER_MUSIC_COMPONENT_VERSION L"1.4.0"

#define ER_DECLARE_MUSIC_COMPONENT_DEFINITION() \
	public: virtual const wchar_t *__stdcall getInformation() const; \
		virtual const Format *__stdcall getFormats() const;

#define ER_DECLARE_MUSIC_COMPONENT(name, ext, fnc, cls) \
	extern "C" IERMusicComponent *newInstance(HWND window) { \
		IERMusicComponent *p = new (cls)(); \
		p->setWindow(window); \
		return p; \
	} \
	extern "C" void deleteInstance(IERMusicComponent *p) { \
		delete p; \
	} \
	const wchar_t * ## cls ## ::getInformation() const { \
		return name L"\0" ext L"\0" fnc L"\0" ER_MUSIC_COMPONENT_VERSION; \
	}

#define ER_FUNCTION_READONLY L"R"
#define ER_FUNCTION_READONLY_INCUE L"RI"
#define ER_FUNCTION_WRITEONLY L"W"
#define ER_FUNCTION_READWRITE "RW"
#define ER_FUNCTION_READWRITE_INCUE L"RWI"

#define ER_DECLARE_MUSIC_COMPONENT_FORMAT_VOID(cls) \
	const cls ## ::Format * ## cls ## ::getFormats() const { \
		static const Format l[] = { \
			{ 0xFFFFFFFFu, NULL } \
		}; \
		return l; \
	}

#define ER_DECLARE_MUSIC_COMPONENT_FORMAT_BEGIN(cls, defaultNumber) \
	const cls ## ::Format * ## cls ## ::getFormats() const { \
		static const Format l[] = { \
			{ defaultNumber, NULL }, \

#define ER_DECLARE_MUSIC_COMPONENT_FORMAT_ITEM(number, name) \
			{ number, name },

#define ER_DECLARE_MUSIC_COMPONENT_FORMAT_END(cls) \
			{ 0xFFFFFFFFu, NULL } \
		}; \
		return l; \
	}

#define ER_EXTENSION_VOID_OUTPUT L"VOID_OUTPUT"
