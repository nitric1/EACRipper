// XXX: No include guard

template<typename Return DELEGATE_TEMPLATE_COMMA DELEGATE_TEMPLATE_PARAM(DELEGATE_NUM_ARG)>
class ERDelegate<Return (DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG))>
{
public:
	virtual ~ERDelegate()
	{
	}

public:
	virtual Return invoke(DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG)) = 0;
	virtual Return operator ()(DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG)) = 0;
	virtual operator bool() = 0;
};

template<typename Return DELEGATE_TEMPLATE_COMMA DELEGATE_TEMPLATE_PARAM(DELEGATE_NUM_ARG)>
class ERDelegateImpl<Return (DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG))> : public ERDelegate<Return (DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG))>
{
private:
	std::function<Return (DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG))> fn;

public:
	ERDelegateImpl()
	{
	}

	ERDelegateImpl(const ERDelegateImpl<Return (DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG))> &obj)
		: fn(obj.fn)
	{
	}

	ERDelegateImpl(ERDelegateImpl<Return (DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG))> &&obj)
		: fn(std::move(obj.fn))
	{
	}

	ERDelegateImpl(Return (*ifn)(DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG)))
		: fn(ifn)
	{
	}

#define DELEGATE_BIND_ARG_ELEMENT(n) std::placeholders::_##n
#define DELEGATE_BIND_ARG(n) DELEGATE_REPEAT(n, DELEGATE_BIND_ARG_ELEMENT, DELEGATE_COMMA)

	template<typename FunctionClass, typename InstanceClass>
	ERDelegateImpl(Return (FunctionClass::*ifn)(DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG)), InstanceClass *ic)
		: fn(std::bind(ifn, ic DELEGATE_TEMPLATE_COMMA DELEGATE_BIND_ARG(DELEGATE_NUM_ARG)))
	{
	}

	template<typename FunctionClass, typename InstanceClass>
	ERDelegateImpl(Return (FunctionClass::*ifn)(DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG)) const, const InstanceClass *ic)
		: fn(std::bind(ifn, ic DELEGATE_TEMPLATE_COMMA DELEGATE_BIND_ARG(DELEGATE_NUM_ARG)))
	{
	}

	template<typename FunctionClass, typename InstanceClass>
	ERDelegateImpl(Return (FunctionClass::*ifn)(DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG)) volatile, volatile InstanceClass *ic)
		: fn(std::bind(ifn, ic DELEGATE_TEMPLATE_COMMA DELEGATE_BIND_ARG(DELEGATE_NUM_ARG)))
	{
	}

	template<typename FunctionClass, typename InstanceClass>
	ERDelegateImpl(Return (FunctionClass::*ifn)(DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG)) const volatile, const volatile InstanceClass *ic)
		: fn(std::bind(ifn, ic DELEGATE_TEMPLATE_COMMA DELEGATE_BIND_ARG(DELEGATE_NUM_ARG)))
	{
	}

	template<typename Functor>
	ERDelegateImpl(Functor ifn)
		: fn(ifn)
	{
	}

	virtual ~ERDelegateImpl()
	{
	}

public:
	ERDelegateImpl<Return (DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG))> &operator =(std::nullptr_t)
	{
		fn.clear();
		return *this;
	}

	ERDelegateImpl<Return (DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG))> &operator =(const ERDelegateImpl<Return (DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG))> &obj)
	{
		fn = obj.fn;
		return *this;
	}

	ERDelegateImpl<Return (DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG))> &operator =(ERDelegateImpl<Return (DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG))> &&obj)
	{
		fn = std::move(obj.fn);
		return *this;
	}

	ERDelegateImpl<Return (DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG))> &operator =(Return (*ifn)(DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG)))
	{
		fn = ifn;
		return *this;
	}

	template<typename FunctionClass, typename InstanceClass>
	ERDelegateImpl<Return (DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG))> &operator =(std::pair<Return (FunctionClass::*)(DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG)), InstanceClass *> ifn)
	{
		fn = std::bind(ifn.first, ifn.second DELEGATE_TEMPLATE_COMMA DELEGATE_BIND_ARG(DELEGATE_NUM_ARG));
		return *this;
	}

	template<typename FunctionClass, typename InstanceClass>
	ERDelegateImpl<Return (DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG))> &operator =(std::pair<Return (FunctionClass::*)(DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG)) const, const InstanceClass *> ifn)
	{
		fn = std::bind(ifn.first, ifn.second DELEGATE_TEMPLATE_COMMA DELEGATE_BIND_ARG(DELEGATE_NUM_ARG));
		return *this;
	}

	template<typename FunctionClass, typename InstanceClass>
	ERDelegateImpl<Return (DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG))> &operator =(std::pair<Return (FunctionClass::*)(DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG)) volatile, volatile InstanceClass *> ifn)
	{
		fn = std::bind(ifn.first, ifn.second DELEGATE_TEMPLATE_COMMA DELEGATE_BIND_ARG(DELEGATE_NUM_ARG));
		return *this;
	}

	template<typename FunctionClass, typename InstanceClass>
	ERDelegateImpl<Return (DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG))> &operator =(std::pair<Return (FunctionClass::*)(DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG)) const volatile, const volatile InstanceClass *> ifn)
	{
		fn = std::bind(ifn.first, ifn.second DELEGATE_TEMPLATE_COMMA DELEGATE_BIND_ARG(DELEGATE_NUM_ARG));
		return *this;
	}

#undef DELEGATE_BIND_ARG_ELEMENT
#undef DELEGATE_BIND_ARG

	template<typename Functor>
	ERDelegateImpl<Return (DELEGATE_TEMPLATE_ARG(DELEGATE_NUM_ARG))> &operator =(Functor ifn)
	{
		fn = ifn;
		return *this;
	}

public:
	virtual Return invoke(DELEGATE_FUNCTION_PARAM(DELEGATE_NUM_ARG))
	{
		return fn(DELEGATE_FUNCTION_ARG(DELEGATE_NUM_ARG));
	}

	virtual Return operator ()(DELEGATE_FUNCTION_PARAM(DELEGATE_NUM_ARG))
	{
		return invoke(DELEGATE_FUNCTION_ARG(DELEGATE_NUM_ARG));
	}

	virtual operator bool()
	{
		return static_cast<bool>(fn);
	}
};
