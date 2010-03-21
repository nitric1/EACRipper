#include "Defaults.h"

#include <vector>
#include <sstream>

#include "Utility.h"
#include "FileStream.h"
#include "Configure.h"

using namespace std;

namespace EACRipper
{
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
		vector<wchar_t> buffer(512);
		size_t pathlen;

		pathlen = GetModuleFileNameW(NULL, &*buffer.begin(), buffer.size());

		wstring path(buffer.begin(), buffer.begin() + pathlen);
		size_t pos = path.rfind(L'\\');

		if(pos == wstring::npos) // What's happened?
			throw(runtime_error("Cannot get configure path."));

		path.erase(++ pos);
		path += L"EACRipper.conf";

		return path;
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

	const wstring &Configure::get(const wstring &name, const wstring &def) const
	{
		map<wstring, wstring>::const_iterator it = find(name);
		if(it == confMap.end())
			return def;
		return it->second;
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
