#pragma once

#include "IERStream.h"

class FileStreamReader : public IERFileStreamReader
{
private:
	HANDLE file;

public:
	FileStreamReader();
	explicit FileStreamReader(const wchar_t *);
	virtual ~FileStreamReader();

public:
	virtual bool open(const wchar_t *, bool = false);
	virtual bool close();
	virtual bool usable() const;
	virtual uint64_t size() const;
	virtual bool seek(int64_t, int32_t);
	virtual uint64_t tell() const;
	virtual size_t read(void *, size_t);
};

class FileStreamWriter : public IERFileStreamWriter
{
private:
	HANDLE file;

public:
	FileStreamWriter();
	explicit FileStreamWriter(const wchar_t *);
	virtual ~FileStreamWriter();

public:
	virtual bool open(const wchar_t *, bool = true);
	virtual bool close();
	virtual bool usable() const;
	virtual uint64_t size() const;
	virtual bool seek(int64_t, int32_t);
	virtual uint64_t tell() const;
	virtual size_t write(const void *, size_t);
};
