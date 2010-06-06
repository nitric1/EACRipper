#pragma once

namespace EACRipper
{
	class CueParser
	{
	private:
		std::map<std::wstring, std::wstring> albumFields;
		std::map<size_t, std::map<std::wstring, std::wstring>> tracks;

	public:
		virtual ~CueParser();

	public:
		bool parse(const std::wstring &);
		bool clear();

		const std::map<std::wstring, std::wstring> &getAlbumFields() const;
		std::wstring getAlbumField(const std::wstring &) const;
		size_t getTrackCount() const;
		const std::map<std::wstring, std::wstring> &getTrackFields(size_t) const;
		std::wstring getTrackField(size_t, const std::wstring &) const;
	};
}
