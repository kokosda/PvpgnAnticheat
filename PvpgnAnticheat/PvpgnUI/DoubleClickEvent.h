#pragma once

#include "Event.h"

namespace UI_Controls_Handlers
{
	class DoubleClickEvent : public Event
	{
		public:
			DoubleClickEvent(void* args, Control* sender);
	};
}