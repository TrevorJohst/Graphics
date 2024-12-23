#include "Windows/Window.h"
#include "Windows/Resource.h"
#include "Graphics/Display.h"
#include "Graphics/Graphics.h"

//////////////////////////////////////////////////////////////////
// Main window that our program will enter through
int CALLBACK wWinMain(
    _In_     HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_     PWSTR     pCmdLine,
    _In_     int       nCmdShow )
{
    // Create our Window class
    Window wnd( 720, 480, L"Graphics" );

    // Create graphics object to pass into display
    Graphics graphics( wnd.GetHandle() );

    // Create display object for user to modify screen
    Display display( graphics );

    // Main loop
    MSG msg;
    BOOL gResult;
    bool running = true;
    while ( running )
    {
        // Main message loop
        while ( gResult = PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
        {
            if ( msg.message == WM_QUIT )
                running = false;

            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }

        display.DisplayFrame();
    }

    // Message loop errored
    if ( gResult == -1 )
        return -1;

    // Exit with window termination return value
    return static_cast<int>(msg.wParam);
}