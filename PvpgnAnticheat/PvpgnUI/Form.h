#pragma once

#include "Control.h"
#include <Windows.h>

using namespace UI_Controls;

#define FORM_DEFAULT_TOPLEFTX_VALUE 100
#define FORM_DEFAULT_TOPLEFTY_VALUE 100
#define FORM_DEFAULT_WIDTH_VALUE 800
#define FORM_DEFAULT_HEIGHT_VALUE 600

namespace UI_Controls
{
	class Form : public Control
	{
		public:
			Form(wstring name, wstring title, bool main = false, int width = FORM_DEFAULT_WIDTH_VALUE, int height = FORM_DEFAULT_HEIGHT_VALUE, 
					int topLeftX = FORM_DEFAULT_TOPLEFTX_VALUE, int topLeftY = FORM_DEFAULT_TOPLEFTY_VALUE);

			void Initialize(CONTROL_WINDOW_PROCEDURE(f) = NULL);
			void Initialize(WNDCLASSEX formDescription);
			wstring Title(void) const;
			void Show(int cmdShow = 1);
			MSG CurrentMessage(void) const;
			void StartEventLoop(void);
			static LRESULT CALLBACK DefaultWindowProcedure(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
			LRESULT CALLBACK WindowProcedure(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

		private:
			static bool hasMain;
			WNDCLASSEX formDescription;
			bool alreadyInitialized;
			wstring title;
			MSG currentMessage;

			void InitializeBase(void);
			void LaunchMessageLoop(void);
			void UpdateChildrenHandles(void);
			bool AlreadyHasMain(void);
	};
}