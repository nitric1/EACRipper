#include "Defaults.h"

#include "Utility.h"
#include "TrackList.h"

using namespace std;

namespace EACRipper
{
	TrackList::AlbumFieldIOProxy::AlbumFieldIOProxy(const TrackList::AlbumFieldIOProxy &proxy)
		: list(proxy.list), field(proxy.field)
	{
	}

	TrackList::AlbumFieldIOProxy::AlbumFieldIOProxy(TrackList *ilist, const wstring &ifield)
		: list(ilist), field(ifield)
	{
	}

	TrackList::AlbumFieldIOProxy::operator wstring() const
	{
		return list->getAlbumField(field);
	}

	wstring TrackList::AlbumFieldIOProxy::operator =(const wstring &value)
	{
		list->setAlbumField(field, value);
		return value;
	}

	TrackList::TrackFieldIOProxy::TrackFieldIOProxy(const TrackList::TrackFieldIOProxy &proxy)
		: list(proxy.list), track(proxy.track), field(proxy.field)
	{
	}

	TrackList::TrackFieldIOProxy::TrackFieldIOProxy(TrackList *ilist, size_t itrack, const wstring &ifield)
		: list(ilist), track(itrack), field(ifield)
	{
	}

	TrackList::TrackFieldIOProxy::operator wstring() const
	{
		return list->getTrackField(track, field);
	}

	wstring TrackList::TrackFieldIOProxy::operator =(const wstring &value)
	{
		list->setTrackField(track, field, value);
		return value;
	}

	TrackList::TrackFieldProxy::TrackFieldProxy(const TrackFieldProxy &proxy)
		: list(proxy.list), track(proxy.track)
	{
	}

	TrackList::TrackFieldProxy::TrackFieldProxy(TrackList *ilist, size_t itrack)
		: list(ilist), track(itrack)
	{
	}

	const TrackList::TrackFieldIOProxy TrackList::TrackFieldProxy::operator [](const wstring &field) const
	{
		return TrackFieldIOProxy(const_cast<TrackList *>(list), track, field);
	}

	TrackList::TrackFieldIOProxy TrackList::TrackFieldProxy::operator [](const wstring &field)
	{
		return TrackFieldIOProxy(list, track, field);
	}

	TrackList::~TrackList()
	{
	}

	const TrackList::AlbumFieldIOProxy TrackList::operator [](const wstring &field) const
	{
		return AlbumFieldIOProxy(const_cast<TrackList *const>(this), field);
	}

	TrackList::AlbumFieldIOProxy TrackList::operator [](const wstring &field)
	{
		return AlbumFieldIOProxy(this, field);
	}

	const TrackList::TrackFieldProxy TrackList::operator [](size_t track) const
	{
		return TrackFieldProxy(const_cast<TrackList *const>(this), track);
	}

	TrackList::TrackFieldProxy TrackList::operator [](size_t track)
	{
		return TrackFieldProxy(this, track);
	}

	CuesheetTrackList::CuesheetTrackList()
	{
	}

	CuesheetTrackList::CuesheetTrackList(const wstring &str)
	{
		parse(str);
	}

	CuesheetTrackList::CuesheetTrackList(const CuesheetTrackList &list)
		: albumFields(list.albumFields), tracks(list.tracks)
	{
	}

	CuesheetTrackList::CuesheetTrackList(CuesheetTrackList &&list)
		: albumFields(move(list.albumFields)), tracks(move(list.tracks))
	{
	}

	CuesheetTrackList::~CuesheetTrackList()
	{
	}

	CuesheetTrackList *CuesheetTrackList::clone() const
	{
		return new CuesheetTrackList(*this);
	}

	bool CuesheetTrackList::parse(const wstring &str)
	{
		if(!close())
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

		map<size_t, map<wstring, wstring>>::iterator be, ed, i, tmp;
		for(be = i = tracks.begin(), ed = tracks.end(); i != ed; ++ i)
		{
			if(i->second.find(L"Start Time") == i->second.end())
			{
				if(i == be)
					i->second[L"Start Time"] = L"00:00:00";
				else
					i->second[L"Start Time"] = (-- (tmp = i))->second[L"End Time"];
			}
			
			if(i->second.find(L"End Time") == i->second.end())
			{
				if(++ (tmp = i) == ed)
					i->second[L"End Time"] = L"EOF";
				else
					i->second[L"End Time"] = (++ (tmp = i))->second[L"Start Time"];
			}
		}

		return true;
	}

	bool CuesheetTrackList::close()
	{
		albumFields.clear();
		tracks.clear();
		return true;
	}

	wstring CuesheetTrackList::getAlbumField(const wstring &field) const
	{
		auto it = albumFields.find(field);
		if(it == albumFields.end())
			return wstring();
		return it->second;
	}

	void CuesheetTrackList::setAlbumField(const wstring &field, const wstring &value)
	{
		albumFields[field] = value;
	}

	size_t CuesheetTrackList::getTrackCount() const
	{
		return tracks.size();
	}

	wstring CuesheetTrackList::getTrackField(size_t track, const wstring &field) const
	{
		auto it = tracks.find(track);
		if(it == tracks.end())
			return wstring();
		auto fit = it->second.find(field);
		if(fit == it->second.end())
			return wstring();
		return fit->second;
	}

	void CuesheetTrackList::setTrackField(size_t track, const wstring &field, const wstring &value)
	{
		auto it = tracks.find(track);
		if(it == tracks.end())
			return;
		it->second[field] = value;
	}
}
