#pragma once

namespace EACRipper
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

#include "DelegateDefinition.h"

#define DELEGATE_NUM_ARG 0
#include "DelegateStub.h"
