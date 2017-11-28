#pragma once

#include "Form.h"
#include "Button.h"
#include "WindowBase.h"

using namespace UI_Controls;

namespace UI_Views
{
	class MainWindow : public WindowBase
	{
		public:
			MainWindow();
			virtual ~MainWindow();
			void Initialize(void);
			int Message(void) const;

		protected:
			Form* mainForm;
			Button* chooseTargetDirectoryButton;
			Button* launchSignatureDetectionButton;
	};
}