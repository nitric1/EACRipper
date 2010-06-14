#include "Defaults.h"

#include "FileStream.h"

using namespace std;

namespace EACRipper
{
	FileStreamReader::FileStreamReader()
		: file(INVALID_HANDLE_VALUE)
	{
	}

	FileStreamReader::FileStreamReader(const wchar_t *name)
		: file(INVALID_HANDLE_VALUE)
	{
		open(name);
	}

	FileStreamReader::~FileStreamReader()
	{
		close();
	}

	bool FileStreamReader::open(const wchar_t *name, bool make)
	{
		if(!close())
			return false;

		file = CreateFileW(name, GENERIC_READ, FILE_SHARE_READ, nullptr, (make ? OPEN_ALWAYS : OPEN_EXISTING), 0, nullptr);
		if(file == INVALID_HANDLE_VALUE)
			return false;

		return true;
	}

	bool FileStreamReader::close()
	{
		if(file == INVALID_HANDLE_VALUE)
			return true;

		if(!CloseHandle(file))
			return false;

		return true;
	}

	bool FileStreamReader::usable() const
	{
		return file != INVALID_HANDLE_VALUE;
	}

	uint64_t FileStreamReader::size() const
	{
		DWORD sizeLow, sizeHigh;
		sizeLow = GetFileSize(file, &sizeHigh);

		return (static_cast<uint64_t>(sizeHigh) << 32) | static_cast<uint64_t>(sizeLow);
	}

	bool FileStreamReader::seek(int64_t pos, int32_t mode)
	{
		long high = static_cast<long>(pos >> 32);
		uint32_t res = SetFilePointer(file, static_cast<long>(pos & 0xFFFFFFFFll), &high, static_cast<unsigned>(mode));
		if(res == INVALID_SET_FILE_POINTER && GetLastError() != ERROR_SUCCESS)
			return false;
		return true;
	}

	uint64_t FileStreamReader::tell() const
	{
		long high = 0;
		uint32_t low = SetFilePointer(file, 0, &high, FILE_CURRENT);
		if(low == INVALID_SET_FILE_POINTER && GetLastError() != ERROR_SUCCESS)
			return numeric_limits<uint64_t>::max();
		return static_cast<uint64_t>(low) | (static_cast<uint64_t>(high) << 32);
	}

	size_t FileStreamReader::read(void *buffer, size_t size)
	{
		DWORD read;
		if(!ReadFile(file, buffer, static_cast<DWORD>(size), &read, nullptr))
			return numeric_limits<size_t>::max();
		return read;
	}

	FileStreamWriter::FileStreamWriter()
		: file(INVALID_HANDLE_VALUE)
	{
	}

	FileStreamWriter::FileStreamWriter(const wchar_t *name)
		: file(INVALID_HANDLE_VALUE)
	{
		open(name);
	}

	FileStreamWriter::~FileStreamWriter()
	{
		close();
	}

	bool FileStreamWriter::open(const wchar_t *name, bool truncate)
	{
		if(!close())
			return false;

		file = CreateFileW(name, GENERIC_WRITE, FILE_SHARE_READ, nullptr, (truncate ? CREATE_ALWAYS : OPEN_ALWAYS), 0, nullptr);
		if(file == INVALID_HANDLE_VALUE)
			return false;

		if(truncate)
			seek(0, END);

		return true;
	}

	bool FileStreamWriter::close()
	{
		if(file == INVALID_HANDLE_VALUE)
			return true;

		if(!CloseHandle(file))
			return false;

		return true;
	}

	bool FileStreamWriter::usable() const
	{
		return file != INVALID_HANDLE_VALUE;
	}

	uint64_t FileStreamWriter::size() const
	{
		DWORD sizeLow, sizeHigh;
		sizeLow = GetFileSize(file, &sizeHigh);

		return (static_cast<uint64_t>(sizeHigh) << 32) | static_cast<uint64_t>(sizeLow);
	}

	bool FileStreamWriter::seek(int64_t pos, int32_t mode)
	{
		long high = static_cast<long>(pos >> 32);
		uint32_t res = SetFilePointer(file, static_cast<long>(pos & 0xFFFFFFFFull), &high, static_cast<unsigned>(mode));
		if(res == INVALID_SET_FILE_POINTER && GetLastError() != ERROR_SUCCESS)
			return false;
		return true;
	}

	uint64_t FileStreamWriter::tell() const
	{
		long high = 0;
		uint32_t low = SetFilePointer(file, 0, &high, FILE_CURRENT);
		if(low == INVALID_SET_FILE_POINTER && GetLastError() != ERROR_SUCCESS)
			return numeric_limits<uint64_t>::max();
		return static_cast<uint64_t>(low) | (static_cast<uint64_t>(high) << 32);
	}

	size_t FileStreamWriter::write(const void *data, size_t size)
	{
		DWORD written;
		if(!WriteFile(file, data, static_cast<DWORD>(size), &written, nullptr))
			return numeric_limits<size_t>::max();
		return written;
	}
}
