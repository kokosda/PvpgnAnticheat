#include "Control.h"
#include <stdarg.h>
#include <Windows.h>

using namespace UI_Controls;

HINSTANCE Control::instance = NULL;

Control::Control(wstring name, int x, int y, int width, int height, wstring text, wstring className, Control *parentControl, vector<long> styles) 
			: className(className), stylesBitMask(0), name(name), topLeftX(x), topLeftY(y), width(width), height(height), text(text), handle(NULL),
				parentControl(parentControl), windowProcedurePointer(NULL)
{
	Styles(styles);
	
	if (this->parentControl != NULL)
	{
		this->parentControl->AddChild(const_cast<Control*>(this));
	}
}

Control::~Control()
{
}

wstring Control::Name(wstring value)
{
	if (value != CONTROL_GETTER_WSTRING_CONST)
		this->name = value;
	
	return this->name;
}

int Control::TopLeftX(int value)
{
	if (value != CONTROL_GETTER_INTEGER_CONST)
	{
		topLeftX = value;
		UpdatePosition();
	}

	return topLeftX;
}

int Control::TopLeftY(int value)
{
	if (value != CONTROL_GETTER_INTEGER_CONST)
	{
		topLeftY = value;
		UpdatePosition();
	}

	return topLeftY;
}

int Control::Width(int value)
{
	if (value != CONTROL_GETTER_INTEGER_CONST)
	{
		width = value;
		UpdatePosition();
	}

	return width;
}

int Control::Height(int value)
{
	if (value != CONTROL_GETTER_INTEGER_CONST)
	{
		height = value;
		UpdatePosition();
	}

	return height;
}

const vector<long> Control::Styles(vector<long> values)
{
	if (values != vector<long>())
	{
		for (unsigned int i = 0; i < values.size(); i++)
		{
			if (!HasStyle(values[i]))
			{
				styles.push_back(values[i]);
				stylesBitMask |= values[i];
				UpdateStyles();
			}
		}
	}

	return styles;
}

void Control::RemoveStyle(long style)
{
	if (HasStyle(style))
	{
		stylesBitMask ^= style;
		styles.erase(FindStyle(style));
		UpdateStyles();
	}
}

bool Control::HasStyle(long style)
{
	vector<long>::iterator it = find(styles.begin(), styles.end(), style);	
	return it != styles.end();
}

vector<long>::iterator Control::FindStyle(long style)
{
	return find(styles.begin(), styles.end(), style);
}

wstring Control::ClassName(void) const
{
	return className;
}

bool Control::IsSystem(void) const
{
	if (className == L"BUTTON")
		return true;
	if (className == L"COMBOBOX")
		return true;
	if (className == L"EDIT")
		return true;
	if (className == L"LISTBOX")
		return true;
	if (className == L"MDICLIENT")
		return true;
	if (className == L"RichEdit")
		return true;
	if (className == L"RICHEDIT_CLASS")
		return true;
	if (className == L"SCROLLBAR")
		return true;
	if (className == L"STATIC")
		return true;

	return false;
}

Control* Control::Parent(Control *value)
{
	if (value != NULL)
	{
		if (parentControl != NULL && parentControl != value)
		{
			parentControl->RemoveChild(const_cast<Control*>(this));
		}
		parentControl = value;
		parentControl->AddChild(const_cast<Control*>(this));
	}
	return parentControl;
}

vector<Control*>* Control::Controls(void)
{
	return &controls;
}

void Control::AddChild(Control *c)
{
	if (!HasChild(c))
	{
		controls.push_back(c);
		c->Styles(MakeStyles(1, WS_CHILD));
		c->Parent(this);
		if (Handle() != NULL)
		{
			SetParent(c->Handle(), Handle());
		}
		//SetWindowLongPtr(c->Handle(), GWLP_WNDPROC, (LONG)windowProcedurePointer);
	}
}

void Control::RemoveChild(Control *c)
{
	if (HasChild(c))
	{
		vector<Control*>::iterator it = find(controls.begin(), controls.end(), c);
		controls.erase(it);
		SetParent(c->Handle(), NULL);
	}
}

void Control::RemoveChild(wstring name)
{
	if (HasChild(name))
	{
		vector<Control*>::iterator controlPlace = FindChild(name);
		SetParent((*controlPlace)->Handle(), NULL);
		controls.erase(controlPlace);
	}
}

bool Control::HasChild(Control *c)
{
	return FindChild(c) != controls.end();
}

bool Control::HasChild(wstring name)
{
	return FindChild(name) != controls.end();
}

vector<Control*>::iterator Control::FindChild(Control *c)
{
	vector<Control*>::iterator it = find(controls.begin(), controls.end(), c);
	return it;
}

vector<Control*>::iterator Control::FindChild(wstring name)
{
	for(vector<Control*>::iterator it = controls.begin(); it != controls.end(); it++)
	{
		if ((*it)->name == name)
			return it;
	}

	return controls.end();
}

vector<Control*>::iterator Control::FindChild(HWND handle)
{
	for(vector<Control*>::iterator it = controls.begin(); it != controls.end(); it++)
	{
		if ((*it)->Handle() == handle)
		{
			return it;
		}
	}

	return controls.end();
}

bool Control::Visible(bool get, bool value)
{
	if (!get)
	{
		if (value)
		{
			Styles(MakeStyles(1, WS_VISIBLE));
		}
		else
		{
			RemoveStyle(WS_VISIBLE);
		}
	}

	return HasStyle(WS_VISIBLE);
}

vector<long> Control::MakeStyles(int argNumber, ...)
{
	va_list argptr;
	va_start(argptr, argNumber);
	int value = -1;
	vector<long> result;
	
	for (int i = 0; i < argNumber; i++)
	{
		value = va_arg(argptr, long);
		result.push_back(value);
	}

	va_end(argptr);
	return result;
}

bool Control::operator==(Control right) const
{
	return name == right.Name();
}

bool Control::operator!=(Control right) const
{
	return name == right.Name();
}

Control Control::operator=(const Control &right) const
{
	return Control(right);
}

wstring Control::Text(wstring value)
{
	if (text == wstring())
	{
		text = value;
	}
	else if (value != CONTROL_GETTER_WSTRING_CONST)
	{
		text = value;
	}

	return text;
}

HINSTANCE Control::ApplicationInstance(HINSTANCE instance)
{
	if (Control::instance == NULL && instance != NULL)
	{
		Control::instance = instance;
	}
	return Control::instance;
}

HWND Control::Handle(HWND handle)
{
	if (handle != NULL)
	{
		this->handle = handle;
	}
	return this->handle;
}

HWND Control::ParentHandle(HWND handle)
{
	if (handle != NULL)
	{
		SetParent(this->Handle(), handle);
	}

	if (Parent() != NULL)
	{
		return Parent()->Handle();
	}

	return NULL;
}

void Control::SetWindowProcedure(CONTROL_WINDOW_PROCEDURE(pointer))
{
	this->windowProcedurePointer = pointer;
}

#pragma region protected

bool Control::CanCreateControl(void) const
{
	return handle == NULL &&
		   ApplicationInstance() != NULL;
}

void Control::CreateControl(wstring text)
{
	if (CanCreateControl())
	{
		HWND parentHandle = NULL;
		if (Parent() != NULL)
		{
			parentHandle = Parent()->Handle();
		}

		wstring controlDisplayingValue = Name();
		if (text != CONTROL_GETTER_WSTRING_CONST)
		{
			controlDisplayingValue = text;
		}

		handle = CreateWindowEx(NULL, ClassName().c_str(), controlDisplayingValue.c_str(), stylesBitMask, TopLeftX(), TopLeftY(), Width(), Height(), 
									parentHandle, NULL, ApplicationInstance(), NULL);
	}
	else
	{
		throw exception("Cannot create control because CanCreateControl returned 'false'");
	}
}

LRESULT CALLBACK Control::WindowProcedure(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (windowProcedurePointer != NULL)
	{
		return windowProcedurePointer(handle, message, wParam, lParam);
	}

	return 0;
}

#pragma endregion

#pragma region private

void Control::UpdatePosition(void)
{
	SetWindowPos(Handle(), NULL, TopLeftX(), TopLeftY(), Width(), Height(), 0);
}

void Control::UpdateStyles(void)
{				
	if (SetWindowLong(Handle(), GWL_STYLE, stylesBitMask) != NULL)
	{
		UpdatePosition();
	}
}

#pragma endregion