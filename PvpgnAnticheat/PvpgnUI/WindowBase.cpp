#include "WindowBase.h"

using namespace UI_Views;

WindowBase::WindowBase()
{
	Control::ApplicationInstance(GetModuleHandle(NULL));
}