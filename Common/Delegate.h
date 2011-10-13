#pragma once

#include "DelegateTemplates.h"

#define _DELEGATE_COMMA ,

#define DELEGATE_REPEAT_0(b, s)
#define DELEGATE_REPEAT_1(b, s) b(1)
#define DELEGATE_REPEAT_2(b, s) DELEGATE_REPEAT_1(b, s) _##s b(2)
#define DELEGATE_REPEAT_3(b, s) DELEGATE_REPEAT_2(b, s) _##s b(3)
#define DELEGATE_REPEAT_4(b, s) DELEGATE_REPEAT_3(b, s) _##s b(4)
#define DELEGATE_REPEAT_5(b, s) DELEGATE_REPEAT_4(b, s) _##s b(5)
#define DELEGATE_REPEAT_6(b, s) DELEGATE_REPEAT_5(b, s) _##s b(6)
#define DELEGATE_REPEAT_7(b, s) DELEGATE_REPEAT_6(b, s) _##s b(7)
#define DELEGATE_REPEAT_8(b, s) DELEGATE_REPEAT_7(b, s) _##s b(8)
#define DELEGATE_REPEAT_9(b, s) DELEGATE_REPEAT_8(b, s) _##s b(9)
#define DELEGATE_REPEAT(n, b, s) DELEGATE_REPEAT_##n(b, s)

#define DELEGATE_TEMPLATE_PARAM_ELEMENT(n) typename Arg##n
#define DELEGATE_TEMPLATE_PARAM(n) DELEGATE_REPEAT(n, DELEGATE_TEMPLATE_PARAM_ELEMENT, DELEGATE_COMMA)

#define DELEGATE_TEMPLATE_ARG_ELEMENT(n) Arg##n
#define DELEGATE_TEMPLATE_ARG(n) DELEGATE_REPEAT(n, DELEGATE_TEMPLATE_ARG_ELEMENT, DELEGATE_COMMA)

#define DELEGATE_FUNCTION_PARAM_ELEMENT(n) Arg##n arg##n
#define DELEGATE_FUNCTION_PARAM(n) DELEGATE_REPEAT(n, DELEGATE_FUNCTION_PARAM_ELEMENT, DELEGATE_COMMA)

#define DELEGATE_FUNCTION_ARG_ELEMENT(n) arg##n
#define DELEGATE_FUNCTION_ARG(n) DELEGATE_REPEAT(n, DELEGATE_FUNCTION_ARG_ELEMENT, DELEGATE_COMMA)

template<typename FunctionType>
class ERDelegate;

template<typename FunctionType>
class ERDelegateImpl;

#define DELEGATE_TEMPLATE_COMMA

#define DELEGATE_NUM_ARG 0
#include "DelegateStub.h"
#undef DELEGATE_NUM_ARG

#undef DELEGATE_TEMPLATE_COMMA
#define DELEGATE_TEMPLATE_COMMA ,

#define DELEGATE_NUM_ARG 1
#include "DelegateStub.h"
#undef DELEGATE_NUM_ARG

#define DELEGATE_NUM_ARG 2
#include "DelegateStub.h"
#undef DELEGATE_NUM_ARG

#define DELEGATE_NUM_ARG 3
#include "DelegateStub.h"
#undef DELEGATE_NUM_ARG

#define DELEGATE_NUM_ARG 4
#include "DelegateStub.h"
#undef DELEGATE_NUM_ARG

#define DELEGATE_NUM_ARG 5
#include "DelegateStub.h"
#undef DELEGATE_NUM_ARG

#define DELEGATE_NUM_ARG 6
#include "DelegateStub.h"
#undef DELEGATE_NUM_ARG

#define DELEGATE_NUM_ARG 7
#include "DelegateStub.h"
#undef DELEGATE_NUM_ARG

#define DELEGATE_NUM_ARG 8
#include "DelegateStub.h"
#undef DELEGATE_NUM_ARG

#define DELEGATE_NUM_ARG 9
#include "DelegateStub.h"
#undef DELEGATE_NUM_ARG
