#include "Defaults.h"

#include "ComponentServiceImpl.h"
#include "FileStream.h"

namespace EACRipper
{
	namespace ServiceImpl
	{
		LocalFile::LocalFile()
			: attr(INVALID_FILE_ATTRIBUTES)
		{
		}

		LocalFile::~LocalFile()
		{
			close();
		}

		bool LocalFile::open(const wchar_t *ipath)
		{
			attr = GetFileAttributesW(ipath);
			path = ipath;
			return true;
		}

		bool LocalFile::close()
		{
			attr = INVALID_FILE_ATTRIBUTES;
			path.clear();
			return true;
		}

		bool LocalFile::exists()
		{
			return attr != INVALID_FILE_ATTRIBUTES;
		}

		bool LocalFile::canRead()
		{
			return attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY;
		}

		bool LocalFile::canWrite()
		{
			// TODO: Creatability in Writability
			return (attr & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY && (attr & FILE_ATTRIBUTE_READONLY) != FILE_ATTRIBUTE_READONLY;
		}

		IERStreamReader *LocalFile::getStreamReader(bool make)
		{
			if(!path.empty())
			{
				FileStreamReader *f = new FileStreamReader();
				if(!f->open(path.c_str(), make))
				{
					delete f;
					return nullptr;
				}
				return ServicePointerManager::instance().append<IERStreamReader>(f);
			}
			return nullptr;
		}

		IERStreamWriter *LocalFile::getStreamWriter(bool truncate)
		{
			if(!path.empty())
			{
				FileStreamWriter *f = new FileStreamWriter();
				if(!f->open(path.c_str(), truncate))
				{
					delete f;
					return nullptr;
				}
				return ServicePointerManager::instance().append<IERStreamWriter>(f);
			}
			return nullptr;
		}

		LocalDirectory::LocalDirectory()
		{
		}

		LocalDirectory::~LocalDirectory()
		{
		}

		bool LocalDirectory::open(const wchar_t *ipath, bool make)
		{
			if(GetFileAttributesW(ipath) == INVALID_FILE_ATTRIBUTES) // Does not exist
			{
				if(make)
				{
					// TODO: Make directory recursively
				}
				else
					return false;
			}

			path = ipath;

			wchar_t last = *path.rbegin();
			if(last == L'/' || last == L'\\')
				path.pop_back();

			return true;
		}

		bool LocalDirectory::close()
		{
			path.clear();
			return true;
		}

		IERFile *LocalDirectory::getFile(const wchar_t *relativePath)
		{
			if(!path.empty())
			{
				LocalFile *f = new LocalFile();
				if(!f->open((path + L"\\" + relativePath).c_str()))
				{
					delete f;
					return nullptr;
				}
				return ServicePointerManager::instance().append<IERFile>(f);
			}
			return nullptr;
		}
	}
}
