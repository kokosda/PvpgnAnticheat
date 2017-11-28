#include "Button.h"

using namespace UI_Controls;

Button::Button(wstring name, wstring text, int topLeftX, int topLeftY, Control* parentControl, int width, int height)
	: Control(name, topLeftX, topLeftY, width, height, text, BUTTON_SYSTEM_CLASSNAME, parentControl)
{
	Styles(MakeStyles(2, BS_PUSHBUTTON, BS_NOTIFY));
	CreateControl(text);
}

ClickEvent* Button::Click(ClickEvent* e)
{
	if (e != NULL)
	{
		click = e;
	}
	return click;
}

DoubleClickEvent* Button::DoubleClick(DoubleClickEvent* e)
{
	if (e != NULL)
	{
		doubleClick = e;
	}

	return doubleClick;
}