#pragma once

#include "Control.h"
#include "ClickEvent.h"
#include "DoubleClickEvent.h"

using namespace std;
using namespace UI_Controls_Handlers;

#define BUTTON_DEFAULT_WIDTH 150
#define BUTTON_DEFAULT_HEIGHT 50
#define BUTTON_SYSTEM_CLASSNAME L"BUTTON"

namespace UI_Controls
{
	class Button : public Control
	{
		public: 
			Button(wstring name, wstring text, int topLeftX, int topLeftY, Control* parentControl = NULL, int width = BUTTON_DEFAULT_WIDTH, int height = BUTTON_DEFAULT_HEIGHT);

			ClickEvent* Click(ClickEvent* e = NULL);
			DoubleClickEvent* DoubleClick(DoubleClickEvent* e = NULL);

		private:
			ClickEvent* click;
			DoubleClickEvent* doubleClick;
	};
}