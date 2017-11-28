#pragma once

#include "../BaseObjects/BaseObject.h"
#include "Control.h"
#include "Event.h"

using namespace BaseObjects;
using namespace UI_Controls;

namespace UI_Controls_Handlers
{
	class Handler : public BaseObject
	{
		public:
			Handler(Event* e);
		
		private:
			Event* e;
	};
}