// XXX: No include guard

#define DELEGATE_CLASS_NAME DelegateImpl##DELEGATE_NUM_ARG
#define DELEGATE_TEMPLATE_LIST DELEGATE_TEMPLATE_LIST_##DELEGATE_NUM_ARG
#define DELEGATE_ARG_LIST DELEGATE_ARG_LIST_##DELEGATE_NUM_ARG
#define DELEGATE_TEMPLATE_ARG DELEGATE_TEMPLATE_ARG_##DELEGATE_NUM_ARG

namespace EACRipper
{
	template<DELEGATE_TEMPLATE_LIST>
	class DelegateImpl<DELEGATE_TEMPLATE_ARG> : public Delegate
#if DELEGATE_NUM_ARG == 1
		, public unary_function<Return, Arg0>
#elif DELEGATE_NUM_ARG == 2
		, public binary_function<Return, Arg0, Arg1>
#endif
	{
	public:
#if DELEGATE_NUM_ARG != 1 && DELEGATE_NUM_ARG != 2
		typedef Return result_type;
#endif

	public:
		Delegate();
		Delegate(std::nullptr_t);
		Delegate(const Delegate<DELEGATE_TEMPLATE_ARG> &);
		Delegate(Delegate<DELEGATE_TEMPLATE_ARG> &&);
		Delegate(Return (*)(DELEGATE_ARG_LIST));
		template<typename FunctionClass>
		Delegate(Return (Class::*)(DELEGATE_ARG_LIST));
		template<typename FunctionClass>
		Delegate(Return (Class::*)(DELEGATE_ARG_LIST) const);
		template<typename FunctionClass>
		Delegate(Return (Class::*)(DELEGATE_ARG_LIST) volatile);
		template<typename FunctionClass, typename InstanceClass>
		Delegate(Return (Class::*)(DELEGATE_ARG_LIST), InstanceClass *);
		template<typename FunctionClass, typename InstanceClass>
		Delegate(Return (Class::*)(DELEGATE_ARG_LIST) const, const InstanceClass *);
		template<typename FunctionClass, typename InstanceClass>
		Delegate(Return (Class::*)(DELEGATE_ARG_LIST) volatile, volatile InstanceClass *);
		Delegate(const std::function<DELEGATE_TEMPLATE_ARG> &);
		virtual ~Delegate();

	public:
	};
}

#undef DELEGATE_TEMPLATE_ARG
#undef DELEGATE_ARG_LIST
#undef DELEGATE_TEMPLATE_LIST
#undef DELEGATE_CLASS_NAME
