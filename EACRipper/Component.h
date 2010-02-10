#pragma once

#include <string>

namespace EACRipper
{
	class Component
	{
	private:
		std::wstring name;

	public:
		explicit Component(const std::wstring &);
		virtual ~Component() = 0;
	};
}
