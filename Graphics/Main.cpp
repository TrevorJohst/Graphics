#include "Graphics.h"
#include "Resource.h"
#include "Display.h"

////////////////////////////////////////////////////////////
//Forward declaration of WindowProc
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

////////////////////////////////////////////////////////////
//Main window that our program will enter through
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	//Store our window name
	const wchar_t CLASS_NAME[] = L"Class Name";

	//Create the window class object
	WNDCLASS wc = {};

	//Initialize window class data
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

	//Register the class
	RegisterClass(&wc);

	//Create the window
	HWND window = CreateWindowEx(
		0,						//Optional window style
		CLASS_NAME,				//Window class
		L"Graphics",			//Window text
		WS_OVERLAPPEDWINDOW,	//Window style

		//Size and Position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,					//Parent window
		NULL,					//Menu
		hInstance,				//Instance handle
		NULL					//Additional application data
	);

	//If the window is null terminate the program
	if (window == NULL)
	{
		return 0;
	}

	//Display the window to the screen
	ShowWindow(window, nCmdShow);

	//Create graphics object to pass into display
	Graphics graphics(window);

	//Create display object for user to modify screen
	Display display(graphics);

	//Temp boolean value to keep the main loop going
	bool running = true;

	//Main loop
	while (running)
	{
		//Create message
		MSG msg = {};

		//Main message loop
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				running = false;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display.DisplayFrame();
	}

	return 0;
}

////////////////////////////////////////////////////////////
//Window behavior function
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//Switch on message type
	switch (uMsg)
	{
		//Destroy the window
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}