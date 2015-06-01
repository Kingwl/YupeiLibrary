#include "MainWindow.h"

int WINAPI wWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	::CoInitialize(nullptr);
	MainWindow* window = new MainWindow();
	window->Run();
}