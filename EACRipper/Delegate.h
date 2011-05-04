#pragma once

#include "DelegateTemplates.h"

namespace EACRipper
{
	namespace Delegate
	{
		class Delegate
		{
		private:

		public:
			virtual ~Delegate();
		};

		template<typename Type>
		class DelegateImpl : public Delegate;
	}

	using Delegate::Delegate;
	using Delegate::DelegateImpl;
}

#include "DelegateDefinition.h"

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
