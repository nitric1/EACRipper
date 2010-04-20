#pragma once

namespace EACRipper
{
	class Version
	{
	private:
		std::wstring version;

	public:
		Version();
		Version(const wchar_t *);
		Version(const std::wstring &);
		virtual ~Version();

	public:
		const std::wstring &getVersion() const;
		void setVersion(const wchar_t *);
		void setVersion(const std::wstring &);

	public:
		Version &operator =(const wchar_t *);
		Version &operator =(const std::wstring &);
		Version &operator =(const Version &);

		bool operator ==(const Version &) const;
		bool operator !=(const Version &) const;
		bool operator <(const Version &) const;
		bool operator <=(const Version &) const;
		bool operator >(const Version &) const;
		bool operator >=(const Version &) const;
	};
}
