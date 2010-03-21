#pragma once

#include <map>
#include <string>

#include "Singleton.h"

namespace EACRipper
{
	class Configure : public Singleton<Configure>
	{
	private:
		std::map<std::wstring, std::wstring> confMap;
		bool changed;

	private:
		Configure();
		~Configure();

	private:
		static std::wstring getConfigurePath();

	private:
		std::map<std::wstring, std::wstring>::iterator find(const std::wstring &);
		std::map<std::wstring, std::wstring>::const_iterator find(const std::wstring &) const;

	public:
		bool exists(const std::wstring &) const;
		const std::wstring &get(const std::wstring &, const std::wstring & = std::wstring()) const;
		void set(const std::wstring &, const std::wstring &);
		void remove(const std::wstring &);

		friend class Singleton<Configure>;
	};
}
