#pragma once

class IERApplication;

class IERApplication
{
public:
	virtual ~IERApplication() = 0;

public:
	virtual HWND getWindow() const = 0;

	virtual bool registerMusicDecoder(const wchar_t *formatName, const wchar_t *ext) = 0;
	virtual bool registerMusicEncoder(const wchar_t *formatName, const wchar_t *ext) = 0;

	virtual bool registerArchiveExtractor(const wchar_t *formatName, const wchar_t *ext) = 0;
};
