#pragma once

namespace EACRipper
{
	class DelegateBase
	{
	public:
		virtual ~DelegateBase() = 0 {}
	};

	template<typename ReturnType, typename ArgTuple>
	class Delegate : public DelegateBase
	{
	public:
		virtual ~Delegate() {}

	public:
		virtual ReturnType run(const ArgTuple &) = 0;
		ReturnType operator()(const ArgTuple &args) { return run(args); }
	};

	template<typename ReturnType, typename ArgTuple, typename Func>
	class DelegateToFunction : public Delegate<ReturnType, ArgTuple>
	{
	};

	template<typename ReturnType, typename ArgTuple, typename Class, typename Func>
	class DelegateToMemberFunction : public Delegate<ReturnType, ArgTuple>
	{
	};
}
