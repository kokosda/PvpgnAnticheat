#pragma once

#include "Control.h"
#include <Windows.h>

using namespace UI_Controls;

namespace UI_Views
{
	class WindowBase : public BaseObject
	{
		public:
			WindowBase();
			virtual void Initialize(void) = 0;
			virtual int Message(void) const = 0;
	};
}