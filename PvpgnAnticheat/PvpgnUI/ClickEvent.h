#pragma once

#include "Event.h"

namespace UI_Controls_Handlers
{
	class ClickEvent : public Event
	{
		public:
			ClickEvent(void* args, Control* sender);
	};
}