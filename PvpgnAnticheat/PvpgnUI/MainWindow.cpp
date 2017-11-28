#include "MainWindow.h"

using namespace UI_Views;

MainWindow::MainWindow()
{
}

MainWindow::~MainWindow()
{
	if (mainForm != NULL)
	{
		delete mainForm;
	}
	if (launchSignatureDetectionButton != NULL)
	{
		delete launchSignatureDetectionButton;
	}
	if (chooseTargetDirectoryButton != NULL)
	{
		delete chooseTargetDirectoryButton;
	}
}

void MainWindow::Initialize(void)
{
	mainForm = new Form(L"mainForm", L"Pvpgn Anticheat", true);	
	launchSignatureDetectionButton = new Button(L"launchSignatureDetection", L"Launch", 10, 10);
	launchSignatureDetectionButton->Visible();
	mainForm->AddChild(launchSignatureDetectionButton);

	mainForm->Initialize();
	mainForm->StartEventLoop();
}
int MainWindow::Message(void) const
{
	return mainForm->CurrentMessage().wParam;
}