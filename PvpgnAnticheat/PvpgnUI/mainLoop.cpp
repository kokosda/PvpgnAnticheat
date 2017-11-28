#pragma once

#include "Control.h"

using namespace UI_Controls;

LRESULT CALLBACK DefaultProc(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(wnd, message, wParam, lParam);
			break;
	}

	return 0;
}