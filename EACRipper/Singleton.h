#pragma once

namespace EACRipper
{
	template<typename T>
	class Singleton
	{
	protected:
		~Singleton() {}

	public:
		static T &instance()
		{
			static T theInstance;
			return theInstance;
		}
	};
}
