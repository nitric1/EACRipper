// XXX: No include guard

#define DELEGATE_CLASS_NAME DelegateImpl##DELEGATE_NUM_ARG
#define DELEGATE_TEMPLATE_LIST DELEGATE_TEMPLATE_LIST_##DELEGATE_NUM_ARG
#define DELEGATE_ARG_LIST DELEGATE_ARG_LIST_##DELEGATE_NUM_ARG
#define DELEGATE_TEMPLATE_ARG DELEGATE_TEMPLATE_ARG_##DELEGATE_NUM_ARG

namespace EACRipper
{
	namespace Delegate
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
			DelegateImpl();
			DelegateImpl(std::nullptr_t);
			DelegateImpl(const DelegateImpl<DELEGATE_TEMPLATE_ARG> &);
			DelegateImpl(DelegateImpl<DELEGATE_TEMPLATE_ARG> &&);
			DelegateImpl(Return (*)(DELEGATE_ARG_LIST));
			template<typename FunctionClass>
			DelegateImpl(Return (Class::*)(DELEGATE_ARG_LIST));
			template<typename FunctionClass>
			DelegateImpl(Return (Class::*)(DELEGATE_ARG_LIST) const);
			template<typename FunctionClass>
			DelegateImpl(Return (Class::*)(DELEGATE_ARG_LIST) volatile);
			template<typename FunctionClass, typename InstanceClass>
			DelegateImpl(Return (Class::*)(DELEGATE_ARG_LIST), InstanceClass *);
			template<typename FunctionClass, typename InstanceClass>
			DelegateImpl(Return (Class::*)(DELEGATE_ARG_LIST) const, const InstanceClass *);
			template<typename FunctionClass, typename InstanceClass>
			DelegateImpl(Return (Class::*)(DELEGATE_ARG_LIST) volatile, volatile InstanceClass *);
			// DelegateImpl(const std::function<DELEGATE_TEMPLATE_ARG> &);
			typename<typename Functor>
			DelegateImpl(Functor fn);
			virtual ~DelegateImpl();

		public:
			DelegateImpl &operator =(std::nullptr_t);
			DelegateImpl &operator =(const DelegateImpl<DELEGATE_TEMPLATE_ARG> &);
			DelegateImpl &operator =(DelegateImpl<DELEGATE_TEMPLATE_ARG> &&);
			DelegateImpl &operator =(Return (*)(DELEGATE_ARG_LIST));
			template<typename FunctionClass>
			DelegateImpl &operator =(Return (Class::*)(DELEGATE_ARG_LIST));
			template<typename FunctionClass>
			DelegateImpl &operator =(Return (Class::*)(DELEGATE_ARG_LIST) const);
			template<typename FunctionClass>
			DelegateImpl &operator =(Return (Class::*)(DELEGATE_ARG_LIST) volatile);
			template<typename FunctionClass, typename InstanceClass>
			DelegateImpl &operator =(Return (Class::*)(DELEGATE_ARG_LIST), InstanceClass *);
			template<typename FunctionClass, typename InstanceClass>
			DelegateImpl &operator =(Return (Class::*)(DELEGATE_ARG_LIST) const, const InstanceClass *);
			template<typename FunctionClass, typename InstanceClass>
			DelegateImpl &operator =(Return (Class::*)(DELEGATE_ARG_LIST) volatile, volatile InstanceClass *);
			// DelegateImpl(const std::function<DELEGATE_TEMPLATE_ARG> &);
			typename<typename Functor>
			DelegateImpl &operator =(Functor fn);
		};
	}
}

#undef DELEGATE_TEMPLATE_ARG
#undef DELEGATE_ARG_LIST
#undef DELEGATE_TEMPLATE_LIST
#undef DELEGATE_CLASS_NAME
