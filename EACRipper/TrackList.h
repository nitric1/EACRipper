#pragma once

namespace EACRipper
{
	class TrackList
	{
	public:
		class AlbumFieldIOProxy
		{
		private:
			TrackList *list;
			std::wstring field;

		private:
			AlbumFieldIOProxy(); // DO NOT IMPLEMENT
			AlbumFieldIOProxy(const AlbumFieldIOProxy &);
			AlbumFieldIOProxy(TrackList *, const std::wstring &);

		private:
			AlbumFieldIOProxy &operator =(const AlbumFieldIOProxy &); // DO NOT IMPLEMENT

		public:
			operator std::wstring() const;
			std::wstring operator =(const std::wstring &);

			friend class TrackList;
		};

		class TrackFieldProxy;

		class TrackFieldIOProxy
		{
		private:
			TrackList *list;
			size_t track;
			std::wstring field;

		private:
			TrackFieldIOProxy(); // DO NOT IMPLEMENT
			TrackFieldIOProxy(const TrackFieldIOProxy &);
			TrackFieldIOProxy(TrackList *, size_t, const std::wstring &);

		private:
			TrackFieldIOProxy &operator =(const TrackFieldIOProxy &); // DO NOT IMPLEMENT

		public:
			operator std::wstring() const;
			std::wstring operator =(const std::wstring &);

			friend class TrackFieldProxy;
		};

		class TrackFieldProxy
		{
		private:
			TrackList *list;
			size_t track;

		private:
			TrackFieldProxy(); // DO NOT IMPLEMENT
			TrackFieldProxy(const TrackFieldProxy &);
			TrackFieldProxy(TrackList *, size_t track);

		private:
			TrackFieldProxy &operator =(const TrackFieldProxy &); // DO NOT IMPLEMENT

		public:
			const TrackFieldIOProxy operator [](const std::wstring &) const;
			TrackFieldIOProxy operator [](const std::wstring &);

			friend class TrackList;
		};

	public:
		virtual ~TrackList() = 0;

	public:
		virtual TrackList *clone() const = 0;

		virtual bool parse(const std::wstring &) = 0;
		virtual bool close() = 0;

		virtual std::wstring getAlbumField(const std::wstring &) const = 0;
		virtual void setAlbumField(const std::wstring &, const std::wstring &) = 0;

		virtual size_t getTrackCount() const = 0;
		virtual std::wstring getTrackField(size_t, const std::wstring &) const = 0;
		virtual void setTrackField(size_t, const std::wstring &, const std::wstring &) = 0;

	public:
		const AlbumFieldIOProxy operator [](const std::wstring &) const;
		AlbumFieldIOProxy operator [](const std::wstring &);
		const TrackFieldProxy operator [](size_t) const;
		TrackFieldProxy operator [](size_t);
	};

	class CuesheetTrackList : public TrackList
	{
	private:
		std::map<std::wstring, std::wstring> albumFields;
		std::map<size_t, std::map<std::wstring, std::wstring>> tracks;

	public:
		CuesheetTrackList();
		explicit CuesheetTrackList(const std::wstring &);
		CuesheetTrackList(const CuesheetTrackList &);
		CuesheetTrackList(CuesheetTrackList &&);
		virtual ~CuesheetTrackList();

	public:
		virtual CuesheetTrackList *clone() const;

		virtual bool parse(const std::wstring &);
		virtual bool close();

		virtual std::wstring getAlbumField(const std::wstring &) const;
		virtual void setAlbumField(const std::wstring &, const std::wstring &);

		virtual size_t getTrackCount() const;
		virtual std::wstring getTrackField(size_t, const std::wstring &) const;
		virtual void setTrackField(size_t, const std::wstring &, const std::wstring &);
	};
}
