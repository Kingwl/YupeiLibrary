#include "WindowBase.h"

namespace Yupei
{
	unique_ptr<WindowManager> WindowManager::windowManager;

	WindowBase::~WindowBase()
	{
		RemoveWindow(windowHandle);
	}

}