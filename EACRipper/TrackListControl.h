#pragma once

#include "EditListControl.h"
#include "TrackList.h"

namespace EACRipper
{
	namespace TrackListFormatter
	{
		class Formatter
		{
		private:
			std::function<std::wstring(const std::wstring &)> fn;

		public:
			Formatter(std::function<std::wstring(const std::wstring &)>);
			Formatter(const Formatter &);
			Formatter(Formatter &&);

		public:
			std::wstring run(const std::wstring &);
			std::wstring operator ()(const std::wstring &);
		};

		class PlainFormatter : public Formatter
		{
		public:
			PlainFormatter();
		};

		class TimeStringFormatter : public Formatter
		{
		};
	}

	class TrackListControl : public EditListControl
	{
	public:
		TrackListControl();
		virtual ~TrackListControl();

	public:
		virtual int32_t addColumn(const std::wstring &, const std::wstring &, TrackListFormatter::Formatter, int32_t,
			int32_t = ALIGN_LEFT, bool = false, const std::wstring & = std::wstring());
		virtual int32_t insertColumn(int32_t, const std::wstring &, const std::wstring &, TrackListFormatter::Formatter, int32_t,
			int32_t = ALIGN_LEFT, bool = false, const std::wstring & = std::wstring());

	public:
		void setList(const TrackList &);
		void clearList();
	};
}
