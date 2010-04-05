#pragma once

#include "ComponentServiceImpl.h"

namespace EACRipper
{
	class Component
	{
	private:
		typedef void (*Initializer)(IERApplication *);
		typedef void (*Uninitializer)();

	private:
		HMODULE library;
		ERApplication app;
		Initializer init;
		Uninitializer uninit;

	public:
		explicit Component(const std::wstring &);
		virtual ~Component();
	};
}
