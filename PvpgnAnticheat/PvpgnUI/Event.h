#pragma once

#include "../BaseObjects/BaseObject.h"
#include "Control.h"
#include "Handler.h"

using namespace UI_Controls;

namespace UI_Controls_Handlers
{
	class Event : public BaseObject
	{
		public:
			Event(void* args, Control* sender);
			virtual void* Args(void* object = NULL);
			virtual Control* Sender(Control* c = NULL);
			virtual Handler* GetHandler(Handler* h = NULL);

			Handler* operator+=(Handler* h);

		protected:
			void* args;
			Control* sender;
			Handler* h;
	};
}