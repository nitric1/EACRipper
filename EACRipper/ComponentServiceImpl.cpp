#include "Defaults.h"

#include "MainController.h"
#include "MainWindow.h"
#include "ComponentServiceImpl.h"
#include "FileStream.h"

using namespace std;
using namespace std::tr1;

namespace EACRipper
{
	ServicePointer::~ServicePointer()
	{
	}

	ServicePointerManager::~ServicePointerManager()
	{
	}

	void ServicePointerManager::remove(void *ptr)
	{
		auto it = find_if(ptrPool.begin(), ptrPool.end(),
			[&ptr](const shared_ptr<ServicePointer> &obj) -> bool { return obj.get()->get() == ptr; });
		if(it != ptrPool.end())
			ptrPool.erase(it);
	}

	ERApplication::~ERApplication()
	{
	}

	void ERApplication::removePointer(void *ptr)
	{
		ServicePointerManager::instance().remove(ptr);
	}

	void *ERApplication::getServicePointerImpl(const ERUUID &uuid, const void *param)
	{
		using namespace ServiceImpl;

		if(uuid == ERServiceUUID<IERServiceMusicDecoderRegister>::uuid)
			return &MusicDecoderRegister::instance();
		else if(uuid == ERServiceUUID<IERServiceInCueMusicDecoderRegister>::uuid)
			return &InCueMusicDecoderRegister::instance();
		else if(uuid == ERServiceUUID<IERServiceMusicEncoderRegister>::uuid)
			return &MusicEncoderRegister::instance();
		else if(uuid == ERServiceUUID<IERServiceStringCodepageConverter>::uuid)
			return appendPointer(new StringCodepageConverter());
		else if(uuid == ERServiceUUID<IERServiceStringCharsetConverter>::uuid)
			return appendPointer(new StringCharsetConverter());
		else if(uuid == ERServiceUUID<IERFileStreamReader>::uuid)
			return appendPointer(new FileStreamReader());
		else if(uuid == ERServiceUUID<IERFileStreamWriter>::uuid)
			return appendPointer(new FileStreamWriter());
		else if(uuid == ERServiceUUID<IERLocalFile>::uuid)
			return appendPointer(new LocalFile());
		else if(uuid == ERServiceUUID<IERLocalDirectory>::uuid)
			return appendPointer(new LocalDirectory());

		return nullptr;
	}

	void ERApplication::removeServicePointerImpl(const ERUUID &uuid, void *ptr)
	{
		removePointer(ptr);
	}

	HWND ERApplication::getWindow() const
	{
		return MainWindow::instance().getWindow();
	}

	const IERComponentInfo *ERApplication::getInfo() const
	{
		return info;
	}

	void ERApplication::setInfo(IERComponentInfo *iinfo)
	{
		info = iinfo;
	}
}
