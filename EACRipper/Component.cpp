#include "Defaults.h"

#include "Component.h"
#include "Version.h"

using namespace std;

namespace EACRipper
{
	const wstring Component::currentSDKVersion = EACRIPPER_COMPONENT_SDK_VERSION;
	const wstring Component::minimumSDKVersion = L"2.0.0";

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

		if(Version(app.getInfo()->getSDKVersion()) < minimumSDKVersion)
			throw(runtime_error("The component's version is lower than supported."));
	}

	Component::~Component()
	{
		uninit();

		FreeLibrary(library);
	}
};
