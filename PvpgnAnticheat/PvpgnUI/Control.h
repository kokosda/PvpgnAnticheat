#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include "../BaseObjects/BaseObject.h"

using namespace BaseObjects;
using namespace std;

#define CONTROL_GETTER_INTEGER_CONST -1
#define CONTROL_GETTER_WSTRING_CONST L""

#define CONTROL_WINDOW_PROCEDURE(pointer) LRESULT(CALLBACK *pointer)(HWND, UINT, WPARAM, LPARAM)

namespace UI_Controls
{
	class Control : public BaseObject
	{
		public:
			Control(wstring name, int topLeftX, int topLeftY, int width, int height, wstring text = CONTROL_GETTER_WSTRING_CONST, wstring className = CONTROL_GETTER_WSTRING_CONST, Control *parentControl = NULL, vector<long> styles = vector<long>());
			~Control();

			wstring Name(wstring value = CONTROL_GETTER_WSTRING_CONST);
			int TopLeftX(int value = CONTROL_GETTER_INTEGER_CONST);
			int TopLeftY(int value = CONTROL_GETTER_INTEGER_CONST);
			int Width(int value = CONTROL_GETTER_INTEGER_CONST);
			int Height(int value = CONTROL_GETTER_INTEGER_CONST);
			const vector<long> Styles(vector<long> values = vector<long>());
			void RemoveStyle(long style);
			bool HasStyle(long style);
			vector<long>::iterator FindStyle(long style);
			wstring ClassName(void) const;
			bool IsSystem(void) const;
			Control* Parent(Control *value = NULL);
			wstring Text(wstring value = CONTROL_GETTER_WSTRING_CONST);

			vector<Control*>* Controls();
			void AddChild(Control *c);
			void RemoveChild(Control *c);
			void RemoveChild(wstring name);
			bool HasChild(Control *c);
			bool HasChild(wstring name);
			vector<Control*>::iterator FindChild(Control *c);
			vector<Control*>::iterator FindChild(wstring name);
			vector<Control*>::iterator FindChild(HWND handle);
			bool Visible(bool get = false, bool value = true);

			static vector<long> MakeStyles(int argNumber, ...);
			static HINSTANCE ApplicationInstance(HINSTANCE instance = NULL);

			bool operator==(Control c) const;
			bool operator!=(Control c) const;
			Control operator=(const Control &c) const;		

			HWND Handle(HWND handle = NULL);
			HWND ParentHandle(HWND handle = NULL);
			void SetWindowProcedure(CONTROL_WINDOW_PROCEDURE(p));

		protected:
			wstring name;
			int topLeftX;
			int topLeftY;
			int width;
			int height;
			vector<long> styles;
			wstring className;
			long stylesBitMask;
			wstring text;
			HWND handle;
			static HINSTANCE instance;

			bool CanCreateControl(void) const;
			virtual void CreateControl(wstring text = CONTROL_GETTER_WSTRING_CONST);
			virtual LRESULT CALLBACK WindowProcedure(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
					
		private:	
			vector<Control*> controls;			
			Control *parentControl;
			CONTROL_WINDOW_PROCEDURE(windowProcedurePointer);

			void UpdatePosition(void);
			void UpdateStyles(void);
	};
}