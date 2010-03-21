#include "Defaults.h"

#include "Component.h"

using namespace std;

namespace EACRipper
{
	Component::Component(const std::wstring &path)
	{
		library = LoadLibraryW(path.c_str());
		if(library == NULL)
			throw(runtime_error("Cannot load component library."));

		init = reinterpret_cast<Initializer>(GetProcAddress(library, "initComponent"));
		uninit = reinterpret_cast<Uninitializer>(GetProcAddress(library, "uninitComponent"));

		if(init == NULL || uninit == NULL)
		{
			FreeLibrary(library);
			throw(runtime_error("Cannot load initComponent or uninitComponent function."));
		}

		init(&app);
	}

	Component::~Component()
	{
		uninit();

		FreeLibrary(library);
	}
};
