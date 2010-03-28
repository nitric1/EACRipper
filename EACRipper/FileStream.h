#pragma once

#include "IERStream.h"

class FileStreamReader : public IERStreamReader
{
private:
	HANDLE file;

public:
	FileStreamReader();
	explicit FileStreamReader(const wchar_t *);
	virtual ~FileStreamReader();

public:
	virtual bool open(const wchar_t *);
	virtual bool close();
	virtual bool usable() const;
	virtual uint64_t size() const;
	virtual bool seek(int64_t, ERStreamSeekMode);
	virtual uint64_t tell() const;
	virtual size_t read(void *, size_t);
};

class FileStreamWriter : public IERStreamWriter
{
private:
	HANDLE file;

public:
	FileStreamWriter();
	explicit FileStreamWriter(const wchar_t *);
	virtual ~FileStreamWriter();

public:
	virtual bool open(const wchar_t *);
	virtual bool close();
	virtual bool usable() const;
	virtual uint64_t size() const;
	virtual bool seek(int64_t, ERStreamSeekMode);
	virtual uint64_t tell() const;
	virtual size_t write(const void *, size_t);
};