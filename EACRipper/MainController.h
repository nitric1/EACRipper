#pragma once

#include "Singleton.h"

namespace EACRipper
{
	class MainController : public Singleton<MainController>
	{
	protected:
		HINSTANCE inst;

	protected:
		MainController();
		~MainController();

	public:
		int run(HINSTANCE);

		friend class Singleton<MainController>;
	};
}
