#pragma once

#include "DelegateTemplates.h"

#define __C(x) typename Arg##x,

#define DELEGATE_TEMPLATE_LIST_0 typename Return
#define DELEGATE_TEMPLATE_LIST_1 typename Return, __C(1)
#define DELEGATE_TEMPLATE_LIST_2 typename Return, __C(1) __C(2)
#define DELEGATE_TEMPLATE_LIST_3 typename Return, __C(1) __C(2) __C(3)
#define DELEGATE_TEMPLATE_LIST_4 typename Return, __C(1) __C(2) __C(3) __C(4)
#define DELEGATE_TEMPLATE_LIST_5 typename Return, __C(1) __C(2) __C(3) __C(4) __C(5)
#define DELEGATE_TEMPLATE_LIST_6 typename Return, __C(1) __C(2) __C(3) __C(4) __C(5) __C(6)
#define DELEGATE_TEMPLATE_LIST_7 typename Return, __C(1) __C(2) __C(3) __C(4) __C(5) __C(6) __C(7)
#define DELEGATE_TEMPLATE_LIST_8 typename Return, __C(1) __C(2) __C(3) __C(4) __C(5) __C(6) __C(7) __C(8)
#define DELEGATE_TEMPLATE_LIST_9 typename Return, __C(1) __C(2) __C(3) __C(4) __C(5) __C(6) __C(7) __C(8) __C(9)

#undef __C

#define DELEGATE_ARG_LIST_0
#define DELEGATE_ARG_LIST_1 Arg1
#define DELEGATE_ARG_LIST_2 Arg1, Arg2
#define DELEGATE_ARG_LIST_3 Arg1, Arg2, Arg3
#define DELEGATE_ARG_LIST_4 Arg1, Arg2, Arg3, Arg4
#define DELEGATE_ARG_LIST_5 Arg1, Arg2, Arg3, Arg4, Arg5
#define DELEGATE_ARG_LIST_6 Arg1, Arg2, Arg3, Arg4, Arg5, Arg6
#define DELEGATE_ARG_LIST_7 Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7
#define DELEGATE_ARG_LIST_8 Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8
#define DELEGATE_ARG_LIST_9 Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9

#define __C(x) Arg##x arg##x

#define DELEGATE_ARG_DETAIL_0
#define DELEGATE_ARG_DETAIL_1 __C(1)
#define DELEGATE_ARG_DETAIL_2 __C(1) __C(2)
#define DELEGATE_ARG_DETAIL_3 __C(1) __C(2) __C(3)
#define DELEGATE_ARG_DETAIL_4 __C(1) __C(2) __C(3) __C(4)
#define DELEGATE_ARG_DETAIL_5 __C(1) __C(2) __C(3) __C(4) __C(5)
#define DELEGATE_ARG_DETAIL_6 __C(1) __C(2) __C(3) __C(4) __C(5) __C(6)
#define DELEGATE_ARG_DETAIL_7 __C(1) __C(2) __C(3) __C(4) __C(5) __C(6) __C(7)
#define DELEGATE_ARG_DETAIL_8 __C(1) __C(2) __C(3) __C(4) __C(5) __C(6) __C(7) __C(8)
#define DELEGATE_ARG_DETAIL_9 __C(1) __C(2) __C(3) __C(4) __C(5) __C(6) __C(7) __C(8) __C(9)

#undef __C

#define DELEGATE_ARG_CALL_0
#define DELEGATE_ARG_CALL_1 arg1
#define DELEGATE_ARG_CALL_2 arg1, arg2
#define DELEGATE_ARG_CALL_3 arg1, arg2, arg3
#define DELEGATE_ARG_CALL_4 arg1, arg2, arg3, arg4
#define DELEGATE_ARG_CALL_5 arg1, arg2, arg3, arg4, arg5
#define DELEGATE_ARG_CALL_6 arg1, arg2, arg3, arg4, arg5, arg6
#define DELEGATE_ARG_CALL_7 arg1, arg2, arg3, arg4, arg5, arg6, arg7
#define DELEGATE_ARG_CALL_8 arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8
#define DELEGATE_ARG_CALL_9 arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9

#define DELEGATE_TEMPLATE_ARG_0 Return()
#define DELEGATE_TEMPLATE_ARG_1 Return(Arg1)
#define DELEGATE_TEMPLATE_ARG_2 Return(Arg1, Arg2)
#define DELEGATE_TEMPLATE_ARG_3 Return(Arg1, Arg2, Arg3)
#define DELEGATE_TEMPLATE_ARG_4 Return(Arg1, Arg2, Arg3, Arg4)
#define DELEGATE_TEMPLATE_ARG_5 Return(Arg1, Arg2, Arg3, Arg4, Arg5)
#define DELEGATE_TEMPLATE_ARG_6 Return(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)
#define DELEGATE_TEMPLATE_ARG_7 Return(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)
#define DELEGATE_TEMPLATE_ARG_8 Return(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)
#define DELEGATE_TEMPLATE_ARG_9 Return(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)

template<typename FunctionType>
class ERDelegate;

#define DELEGATE_NUM_ARG 0
#include "DelegateStub.h"

#undef DELEGATE_NUM_ARG
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
