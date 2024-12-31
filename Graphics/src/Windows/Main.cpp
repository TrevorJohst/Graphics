#include "User/App.h"
#include "Utility/GraphicsException.h"

//////////////////////////////////////////////////////////////////
// Main window that our program will enter through
int CALLBACK wWinMain(
    _In_     HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_     PWSTR     pCmdLine,
    _In_     int       nCmdShow )
{
    // Error handling
    try
    {
        return App{}.Run();
    }
    catch ( const GraphicsException& e )
    {
        MessageBoxA(
            nullptr,                    // Parent window, was destroyed if here
            e.what(),                   // Message box text
            e.GetType(),                // Message box title
            MB_OK | MB_ICONEXCLAMATION  // Style flags
        );
    }
    catch ( const std::exception& e )
    {
        MessageBoxA(
            nullptr,                    // Parent window, was destroyed if here
            e.what(),                   // Message box text
            "Standard Exception",       // Message box title
            MB_OK | MB_ICONEXCLAMATION  // Style flags
        );
    }
    catch ( ... )
    {
        MessageBoxA(
            nullptr,                    // Parent window, was destroyed if here
            "No details available",     // Message box text
            "Unknown Exception",        // Message box title
            MB_OK | MB_ICONEXCLAMATION  // Style flags
        );
    }
    return -1;
}