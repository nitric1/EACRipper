#include "Defaults.h"

#include "Utility.h"
#include "FileStream.h"
#include "Configure.h"

using namespace std;

namespace EACRipper
{
	const wstring Configure::confFileName = L"EACRipper.conf";

	Configure::Configure()
		: changed(false)
	{
		FileStreamReader r(getConfigurePath().c_str());
		if(!r.usable())
			return;

		vector<wchar_t> buffer(static_cast<size_t>(r.size()) / sizeof(wchar_t));
		r.read(&*buffer.begin(), static_cast<size_t>(r.size()));
		buffer.push_back(L'\0');
		vector<wchar_t>::iterator it = buffer.begin();
		if(*it == L'\xFEFF')
			++ it;

		wstring str(&*it), name, value;
		wistringstream is(str);
		size_t pos;

		while(!is.eof())
		{
			getline(is, str);

			pos = str.find(L'=');
			if(pos == wstring::npos)
				continue;

			name.assign(str.begin(), str.begin() + pos);
			value.assign(str.begin() + pos + 1, str.end());

			trim(name);
			trim(value);

			set(name, value);
		}
	}

	Configure::~Configure()
	{
		if(!changed)
			return;

		wstring str(L"\xFEFF");
		for(map<wstring, wstring>::iterator it = confMap.begin(); it != confMap.end(); ++ it)
		{
			str += it->first;
			str += L"=";
			str += it->second;
			str += L"\n";
		}

		FileStreamWriter w(getConfigurePath().c_str());
		if(!w.usable())
			return;

		w.write(str.c_str(), str.size() * sizeof(wchar_t));
	}

	wstring Configure::getConfigurePath()
	{
		return getCurrentDirectoryPath() + confFileName;
	}

	uint8_t Configure::hexDigit(wchar_t ch)
	{
		if(ch >= L'0' && ch <= L'9')
			return static_cast<uint8_t>(ch - L'0');
		else if(ch >= L'A' && ch <= L'F')
			return static_cast<uint8_t>(ch - L'A' + 10);
		else if(ch >= L'a' && ch <= L'f')
			return static_cast<uint8_t>(ch - L'a' + 10);
		return 0xFF;
	}

	wchar_t Configure::digitHex(uint8_t low)
	{
		static wchar_t digits[16] = {L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'A', L'B', L'C', L'D', L'E', L'F'};
		return digits[low];
	}

	map<wstring, wstring>::iterator Configure::find(const wstring &name)
	{
		return confMap.find(name);
	}

	map<wstring, wstring>::const_iterator Configure::find(const wstring &name) const
	{
		return confMap.find(name);
	}

	bool Configure::exists(const wstring &name) const
	{
		return find(name) != confMap.end();
	}

	wstring Configure::get(const wstring &name, const wstring &def) const
	{
		map<wstring, wstring>::const_iterator it = find(name);
		if(it == confMap.end())
			return def;
		return it->second;
	}

	vector<uint8_t> Configure::getBinary(const wstring &name) const
	{
		wstring str = get(name);
		vector<uint8_t> ve;
		uint8_t v = 0;
		bool next = false;

		ve.reserve(str.size() / 2);

		for(wstring::iterator it = str.begin(); it != str.end(); ++ it)
		{
			if(next)
			{
				v <<= 4;
				v |= hexDigit(*it);
				ve.push_back(v);
				next = false;
			}
			else
			{
				v = hexDigit(*it);
				next = true;
			}
		}

		return ve;
	}

	void Configure::set(const wstring &name, const wstring &value)
	{
		confMap[name] = value;
		changed = true;
	}

	void Configure::remove(const wstring &name)
	{
		map<wstring, wstring>::iterator it = find(name);
		if(it != confMap.end())
		{
			confMap.erase(it);
			changed = true;
		}
	}
}
