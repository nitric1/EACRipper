#pragma once

namespace ERDelegateUtils
{
	template<bool Condition, typename True, typename False> class If;
	template<typename True, typename False> class If<true, True, False> { public: typedef True Result; };
	template<typename True, typename False> class If<false, True, False> { public: typedef False Result; };

	template<typename T, typename U> class IsSameType { public: enum { result = false }; };
	template<typename T> class IsSameType<T, T> { public: enum { result = true }; };

	template<typename Functor>
	bool assertIsFunctor()
	{
		static_assert(sizeof(&Functor::operator()) > 0);
		return true;
	}
}
