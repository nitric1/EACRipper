#pragma once

#include "Singleton.h"

namespace EACRipper
{
	class Configure : public Singleton<Configure>
	{
	private:
		std::map<std::wstring, std::wstring> confMap;
		static const std::wstring confFileName;
		bool changed;

	private:
		Configure();
		~Configure();

	private:
		static std::wstring getConfigurePath();
		static uint8_t hexDigit(wchar_t);
		static wchar_t digitHex(uint8_t);

	private:
		std::map<std::wstring, std::wstring>::iterator find(const std::wstring &);
		std::map<std::wstring, std::wstring>::const_iterator find(const std::wstring &) const;

	public:
		bool exists(const std::wstring &) const;
		std::wstring get(const std::wstring &, const std::wstring & = std::wstring()) const;
		std::vector<uint8_t> getBinary(const std::wstring &) const;
		void set(const std::wstring &, const std::wstring &);
		template<typename Alloc>
		void setBinary(const std::wstring &name, const std::vector<uint8_t, Alloc> &ve)
		{
			std::wstring str;
			str.reserve(ve.size() * 2);
			for(std::vector<uint8_t, Alloc>::const_iterator it = ve.begin(); it != ve.end(); ++ it)
			{
				str.push_back(digitHex(*it >> 4));
				str.push_back(digitHex(*it & 0x0F));
			}
			set(name, str);
		}
		void remove(const std::wstring &);

		friend class Singleton<Configure>;
	};
}
