#include "Form.h"
#include "Button.h"

using namespace UI_Controls;

bool Form::hasMain = false;

LRESULT CALLBACK DefaultProc(HWND, UINT, WPARAM, LPARAM);

Form::Form(wstring name, wstring title, bool main, int width, int height, int topLeftX, int topLeftY)
		: Control(name, topLeftX, topLeftY, width, height),
			title(title), alreadyInitialized(false)
{
	className = L"win32form";
	WstringStream() << className << "_" << name;

	if (!AlreadyHasMain() && main)
	{
		Form::hasMain = main;
		WstringStream(false) << "_" << "main";
	}
	else if (AlreadyHasMain() && main)
	{
		throw exception("Cannot initialize multiple main forms");
	}
	className = WstreamBuffer();
	SetWindowProcedure(Form::DefaultWindowProcedure);
}

void Form::Initialize(CONTROL_WINDOW_PROCEDURE(f))
{
	formDescription.cbSize = sizeof(WNDCLASSEX);
	formDescription.style = CS_HREDRAW | CS_VREDRAW;
	formDescription.lpfnWndProc = f != NULL ? f : DefaultWindowProcedure;
	formDescription.cbClsExtra = 0;
	formDescription.cbWndExtra = 0;
	formDescription.hInstance = ApplicationInstance();
	formDescription.hIcon = LoadIcon(instance, MAKEINTRESOURCE(IDI_APPLICATION));
	formDescription.hCursor = LoadCursor(instance, MAKEINTRESOURCE(IDC_ARROW));
	formDescription.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	formDescription.lpszMenuName = NULL;
	formDescription.lpszClassName = className.c_str();
	formDescription.hIconSm = LoadIcon(ApplicationInstance(), MAKEINTRESOURCE(IDI_APPLICATION));

	InitializeBase();
}

void Form::Initialize(WNDCLASSEX formDescription)
{
	this->formDescription = formDescription;

	InitializeBase();
}

wstring Form::Title(void) const
{
	return title;
}

void Form::Show(int cmdShow)
{
	ShowWindow(Control::handle, cmdShow);
}

MSG Form::CurrentMessage(void) const
{
	return currentMessage;
}

void Form::StartEventLoop(void)
{
	Show();
	UpdateWindow(Control::handle);

	LaunchMessageLoop();
}

#pragma region private

void Form::InitializeBase(void)
{
	if (!alreadyInitialized)
	{
		alreadyInitialized = true;
	}
	else
	{
		throw exception("Cannot twicely initialize form");
	}

	if (!RegisterClassEx(&formDescription))
	{
		throw exception("Cannot register window class");
	}

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (TopLeftX() != FORM_DEFAULT_TOPLEFTX_VALUE)
	{
		Control::topLeftX = screenWidth / 2 - Control::Width() / 2;
	}
	if (TopLeftY() != FORM_DEFAULT_TOPLEFTY_VALUE)
	{
		Control::topLeftY = screenHeight / 2 - Control::Height() / 2;
	}

	Control::handle = CreateWindowEx(WS_EX_APPWINDOW, ClassName().c_str(), Title().c_str(), WS_OVERLAPPEDWINDOW, TopLeftX(), TopLeftY(), Width(), Height(), 
								NULL, NULL, ApplicationInstance(), this);

	if (!Control::handle)
	{
		long errorCode = GetLastError();
		throw exception("Call to CreateWindow failed");
	}

	UpdateChildrenHandles();
}

void Form::LaunchMessageLoop(void)
{
	while(GetMessage(&currentMessage, NULL, 0, 0))
	{
		TranslateMessage(&currentMessage);
		DispatchMessage(&currentMessage);
	}
}

void Form::UpdateChildrenHandles(void)
{
	for(vector<Control*>::iterator it = Controls()->begin(); it != Controls()->end(); ++it)
	{
		(*it)->ParentHandle(handle);
	}
}

bool Form::AlreadyHasMain(void)
{
	return Form::hasMain;
}

LRESULT CALLBACK Form::DefaultWindowProcedure(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NCCREATE)
	{
		LPCREATESTRUCT lpc;
		lpc = (LPCREATESTRUCT)lParam;
		SetWindowLong(handle, GWL_USERDATA, (LONG)lpc->lpCreateParams);
	}
	else if (message == WM_COMMAND)
	{					
		HWND button = (HWND)lParam;
		Form *f = (Form*)GetWindowLong(handle, GWL_USERDATA);
		Control *c = (Control*)(*f->FindChild(button));
	}

	Form *f = (Form*)GetWindowLong(handle, GWL_USERDATA);
	if (f)
	{
		return f->WindowProcedure(handle, message, wParam, lParam);
	}
		
	return DefWindowProc(handle, message, wParam, lParam);
}

LRESULT CALLBACK Form::WindowProcedure(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case BN_CLICKED:
			{
				int a = 0;
				a += 2;
			}
			break;
		default:
			return DefWindowProc(handle, message, wParam, lParam);
	}

	return 0;
}

#pragma endregion