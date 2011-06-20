// XXX: No include guard

#define DELEGATE_CLASS_NAME ERDelegate##DELEGATE_NUM_ARG
#define DELEGATE_IMPL_CLASS_NAME ERDelegateImpl##DELEGATE_NUM_ARG
#define DELEGATE_TEMPLATE_LIST DELEGATE_TEMPLATE_LIST_##DELEGATE_NUM_ARG
#define DELEGATE_ARG_LIST DELEGATE_ARG_LIST_##DELEGATE_NUM_ARG
#define DELEGATE_ARG_DETAIL DELEGATE_ARG_DETAIL_##DELEGATE_NUM_ARG
#define DELEGATE_ARG_CALL DELEGATE_ARG_CALL_##DELEGATE_NUM_ARG
#define DELEGATE_TEMPLATE_ARG DELEGATE_TEMPLATE_ARG_##DELEGATE_NUM_ARG

template<DELEGATE_TEMPLATE_LIST>
class DELEGATE_CLASS_NAME
{
public:
	virtual Return run(DELEGATE_ARG_LIST) = 0;
};

template<DELEGATE_TEMPLATE_LIST>
class DELEGATE_IMPL_CLASS_NAME : public DELEGATE_CLASS_NAME<DELEGATE_ARG_LIST>
{
public:
	DELEGATE_IMPL_CLASS_NAME();
	DELEGATE_IMPL_CLASS_NAME(std::nullptr_t);
	DELEGATE_IMPL_CLASS_NAME(const DELEGATE_IMPL_CLASS_NAME<DELEGATE_TEMPLATE_ARG> &);
	DELEGATE_IMPL_CLASS_NAME(DelegateImpl<DELEGATE_TEMPLATE_ARG> &&);
	DELEGATE_IMPL_CLASS_NAME(Return (*)(DELEGATE_ARG_LIST));
	/*template<typename FunctionClass>
	DELEGATE_IMPL_CLASS_NAME(Return (Class::*)(DELEGATE_ARG_LIST));
	template<typename FunctionClass>
	DELEGATE_IMPL_CLASS_NAME(Return (Class::*)(DELEGATE_ARG_LIST) const);
	template<typename FunctionClass>
	DELEGATE_IMPL_CLASS_NAME(Return (Class::*)(DELEGATE_ARG_LIST) volatile);*/
	template<typename FunctionClass, typename InstanceClass>
	DELEGATE_IMPL_CLASS_NAME(Return (Class::*)(DELEGATE_ARG_LIST), InstanceClass *);
	template<typename FunctionClass, typename InstanceClass>
	DELEGATE_IMPL_CLASS_NAME(Return (Class::*)(DELEGATE_ARG_LIST) const, const InstanceClass *);
	template<typename FunctionClass, typename InstanceClass>
	DELEGATE_IMPL_CLASS_NAME(Return (Class::*)(DELEGATE_ARG_LIST) volatile, volatile InstanceClass *);
	// DELEGATE_IMPL_CLASS_NAME(const std::function<DELEGATE_TEMPLATE_ARG> &);
	typename<typename Functor>
	DELEGATE_IMPL_CLASS_NAME(Functor fn);
	virtual ~DELEGATE_IMPL_CLASS_NAME();

public:
	DELEGATE_IMPL_CLASS_NAME &operator =(std::nullptr_t);
	DELEGATE_IMPL_CLASS_NAME &operator =(const DELEGATE_IMPL_CLASS_NAME<DELEGATE_TEMPLATE_ARG> &);
	DELEGATE_IMPL_CLASS_NAME &operator =(DELEGATE_IMPL_CLASS_NAME<DELEGATE_TEMPLATE_ARG> &&);
	DELEGATE_IMPL_CLASS_NAME &operator =(Return (*)(DELEGATE_ARG_LIST));
	/*template<typename FunctionClass>
	DELEGATE_IMPL_CLASS_NAME &operator =(Return (Class::*)(DELEGATE_ARG_LIST));
	template<typename FunctionClass>
	DELEGATE_IMPL_CLASS_NAME &operator =(Return (Class::*)(DELEGATE_ARG_LIST) const);
	template<typename FunctionClass>
	DELEGATE_IMPL_CLASS_NAME &operator =(Return (Class::*)(DELEGATE_ARG_LIST) volatile);*/
	template<typename FunctionClass, typename InstanceClass>
	DELEGATE_IMPL_CLASS_NAME &operator =(Return (Class::*)(DELEGATE_ARG_LIST), InstanceClass *);
	template<typename FunctionClass, typename InstanceClass>
	DELEGATE_IMPL_CLASS_NAME &operator =(Return (Class::*)(DELEGATE_ARG_LIST) const, const InstanceClass *);
	template<typename FunctionClass, typename InstanceClass>
	DELEGATE_IMPL_CLASS_NAME &operator =(Return (Class::*)(DELEGATE_ARG_LIST) volatile, volatile InstanceClass *);
	// DELEGATE_IMPL_CLASS_NAME(const std::function<DELEGATE_TEMPLATE_ARG> &);
	typename<typename Functor>
	DELEGATE_IMPL_CLASS_NAME &operator =(Functor fn);

public:
	virtual Return run(DELEGATE_ARG_DETAIL);
};

template<DELEGATE_TEMPLATE_LIST>
class ERDelegate<DELEGATE_TEMPLATE_ARG> : public DELEGATE_CLASS_NAME
{
};

#undef DELEGATE_TEMPLATE_ARG
#undef DELEGATE_ARG_LIST
#undef DELEGATE_TEMPLATE_LIST
#undef DELEGATE_CLASS_NAME
