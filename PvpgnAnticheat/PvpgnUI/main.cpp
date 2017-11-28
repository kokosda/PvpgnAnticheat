#pragma once

#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "MainWindow.h"

using namespace UI_Views;

LRESULT CALLBACK DefaultProc2(HWND wnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	MainWindow mainWindowInstance = MainWindow();
	mainWindowInstance.Initialize();

	return mainWindowInstance.Message();
}