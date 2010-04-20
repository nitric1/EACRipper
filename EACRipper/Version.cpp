#include "Defaults.h"

#include "Version.h"

using namespace std;

namespace EACRipper
{
	Version::Version()
	{
	}

	Version::Version(const wchar_t *iversion)
		: version(iversion)
	{
	}

	Version::Version(const wstring &iversion)
		: version(iversion)
	{
	}

	Version::~Version()
	{
	}

	const wstring &Version::getVersion() const
	{
		return version;
	}

	void Version::setVersion(const wchar_t *iversion)
	{
		version = iversion;
	}

	void Version::setVersion(const wstring &iversion)
	{
		version = iversion;
	}

	Version &Version::operator =(const wchar_t *iversion)
	{
		setVersion(iversion);
		return *this;
	}

	Version &Version::operator =(const wstring &iversion)
	{
		setVersion(iversion);
		return *this;
	}

	Version &Version::operator =(const Version &ver)
	{
		version = ver.version;
		return *this;
	}

	// TODO: Implement version compare operators.
	bool Version::operator ==(const Version &ver) const
	{
		return version == ver.version;
	}

	bool Version::operator !=(const Version &ver) const
	{
		return !(*this == ver);
	}

	bool Version::operator <(const Version &ver) const
	{
		return version < ver.version;
	}

	bool Version::operator <=(const Version &ver) const
	{
		return !(*this > ver);
	}

	bool Version::operator >(const Version &ver) const
	{
		return version > ver.version;
	}

	bool Version::operator >=(const Version &ver) const
	{
		return !(*this < ver);
	}
}
