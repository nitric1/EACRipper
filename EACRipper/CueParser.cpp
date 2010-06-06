#include "Defaults.h"

#include "Utility.h"
#include "CueParser.h"

using namespace std;

namespace EACRipper
{
	CueParser::~CueParser()
	{
	}

	bool CueParser::parse(const wstring &str)
	{
		if(!clear())
			return false;

		wistringstream is(str);
		wstring line, command, value, temp;
		wstring::size_type pos, bpos;
		bool inTrack = false, trackBegin = false;
		size_t track, prevTrack = 0;
		uint32_t index;
		wchar_t buf[20];
		wchar_t *end;

		while(!is.eof())
		{
			getline(is, line);
			trim(line);

			bpos = 0;
			pos = line.find(L' ');
			if(pos == wstring::npos)
				continue;
			command = line.substr(bpos, pos - bpos);

			if(command == L"REM")
			{
				bpos = ++ pos;
				pos = line.find(L' ', pos);

				if(pos == wstring::npos || pos == bpos)
					continue;
				else
					command = line.substr(bpos, pos - bpos);
			}

			if(line[++ pos] == L'"')
			{
				bpos = ++ pos;
				pos = line.find(L'"', pos);
				if(pos == wstring::npos)
					continue;
				value = line.substr(bpos, pos - bpos);
			}
			else
				value = line.substr(pos);

			if(inTrack) // track record
			{
				map<wstring, wstring> &cur = tracks[track];

				if(trackBegin)
				{
					_ultow_s(static_cast<unsigned long>(track), buf, 10);
					cur[L"Track"] = buf;
					trackBegin = false;
				}

				if(command == L"PERFORMER")
					cur[L"Artist"] = value;
				else if(command == L"TITLE")
					cur[L"Title"] = value;
				else if(command == L"COMPOSER")
					cur[L"Composer"] = value;
				else if(command == L"ISRC")
					cur[L"ISRC"] = value;
				else if(command == L"INDEX")
				{
					index = wcstoul(value.c_str(), &end, 10);
					for(; *end == L' '; ++ end);
					temp = value.substr(end - value.c_str());

					if(index == 0 && prevTrack > 0)
						tracks[prevTrack][L"End Time"] = temp;
					else if(index == 1)
						cur[L"Start Time"] = temp;
				}
				else if(command == L"TRACK") // New Track
				{
					prevTrack = track;
					track = wcstoul(value.c_str(), nullptr, 10);
					trackBegin = true;
				}
			}
			else
			{
				if(command == L"FILE")
					albumFields[L"File"] = value;
				else if(command == L"PERFORMER")
					albumFields[L"Album Artist"] = value;
				else if(command == L"TITLE")
					albumFields[L"Album Title"] = value;
				else if(command == L"DISCID")
					albumFields[L"DiscID"] = value;
				else if(command == L"GENRE")
					albumFields[L"Genre"] = value;
				else if(command == L"DATE")
					albumFields[L"Date"] = value;
				else if(command == L"TRACK")
				{
					track = wcstoul(value.c_str(), nullptr, 10);
					inTrack = true;
					trackBegin = true;
				}
			}
		}

		return true;
	}

	bool CueParser::clear()
	{
		albumFields.clear();
		tracks.clear();
		return true;
	}

	const map<wstring, wstring> &CueParser::getAlbumFields() const
	{
		return albumFields;
	}

	wstring CueParser::getAlbumField(const wstring &field) const
	{
		auto it = albumFields.find(field);
		if(it == albumFields.end())
			return wstring();
		return it->second;
	}

	size_t CueParser::getTrackCount() const
	{
		return tracks.size();
	}

	const map<wstring, wstring> &CueParser::getTrackFields(size_t track) const
	{
		auto it = tracks.find(track);
		if(it == tracks.end())
			throw(runtime_error("The track does not exists."));
		return it->second;
	}

	wstring CueParser::getTrackField(size_t track, const wstring &field) const
	{
		auto it = tracks.find(track);
		if(it == tracks.end())
			return wstring();
		auto fit = it->second.find(field);
		if(fit == it->second.end())
			return wstring();
		return fit->second;
	}
}
