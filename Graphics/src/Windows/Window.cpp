#include "Windows/Window.h"

// ===============================
// Window::WindowClass definitions
// ===============================

// wndClass singleton declaration
Window::WindowClass Window::WindowClass::wndClass;

//////////////////////////////////////////////////////////////////
// Access the window's name
LPCWSTR Window::WindowClass::GetName() noexcept { return wndClassName; }

//////////////////////////////////////////////////////////////////
// Access the window's instance handle
HINSTANCE Window::WindowClass::GetInstance() noexcept { return wndClass.hInst; }

//////////////////////////////////////////////////////////////////
// Constructs a single window 
Window::WindowClass::WindowClass() noexcept
    :
    hInst( GetModuleHandle( nullptr ) )
{    
    // Create the window class object
    WNDCLASSEX wc = { 0 };

    // Initialize window class data
    wc.cbSize = sizeof( wc );
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = LoadIcon( GetInstance(), MAKEINTRESOURCE(IDI_ICON1));
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetName();
    wc.hIconSm = nullptr;

    // Register the class
    RegisterClassEx( &wc );
}

//////////////////////////////////////////////////////////////////
// Destroys a window on exit 
Window::WindowClass::~WindowClass() { UnregisterClass( wndClassName, GetInstance() ); }

// ==================
// Window definitions
// ==================

//////////////////////////////////////////////////////////////////
// Creates a single window with desired parameters
Window::Window( 
    int            clientWidth, 
    int            clientHeight,
    const wchar_t* name,
    bool           fullscreen ) noexcept
    :
    fullscreen(fullscreen)
{
    // Hard coded window style
    const auto STYLE = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

    // Calculate full window sized based on desired client size
    RECT wr = {};
    wr.left = 0;
    wr.right = clientWidth;
    wr.top = 0;
    wr.bottom = clientHeight;
    AdjustWindowRect( &wr, STYLE, FALSE );

    // Extract full width and height
    width = wr.right;
    height = wr.bottom;

    // Create the window
    hWnd = CreateWindow(
        WindowClass::GetName(),         // Window class name
        name,				            // Window text
        STYLE,                          // Window style
        CW_USEDEFAULT, CW_USEDEFAULT,   // Position
        width, height,  		        // Size
        NULL,					        // Parent window
        NULL,					        // Menu
        WindowClass::GetInstance(),	    // Instance handle
        this					        // Additional application data
    );

    // Show the window
    ShowWindow( hWnd, SW_SHOWDEFAULT );
}

//////////////////////////////////////////////////////////////////
// Destroys the window freeing the instance
Window::~Window() { DestroyWindow( hWnd ); }

//////////////////////////////////////////////////////////////////
// Access the window's handle
HWND Window::GetHandle() noexcept { return hWnd; }

//////////////////////////////////////////////////////////////////
// Sets up message handling at time of window creation
LRESULT Window::HandleMsgSetup( 
    HWND   hWnd, 
    UINT   uMsg,
    WPARAM wParam, 
    LPARAM lParam ) noexcept
{
    if ( uMsg == WM_NCCREATE )
    {
        // Extract the pointer to the Window class from additional creation data
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>( lParam );
        Window* const pWnd = static_cast<Window*>( pCreate->lpCreateParams );

        // Store the pointer to this Window class in the window user data (for future messages)
        SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( pWnd ) );

        // Change the WindowProc function to the thunk now that setup is finished
        SetWindowLongPtr( hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>( &Window::HandleMsgThunk ) );

        // Forward this message to the class handler to finish processing
        return pWnd->HandleMsg( hWnd, uMsg, wParam, lParam );
    }

    // Handle any messages prior to WM_NCCREATE with default handler
    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

//////////////////////////////////////////////////////////////////
// Passes message handling to HandleMsg at runtime
LRESULT Window::HandleMsgThunk( 
    HWND   hWnd, 
    UINT   uMsg,
    WPARAM wParam, 
    LPARAM lParam ) noexcept
{
    // Extract the pointer to the Window class from user data
    Window* const pWnd = reinterpret_cast<Window*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );

    // Forward the message to the class handler
    return pWnd->HandleMsg( hWnd, uMsg, wParam, lParam );
}

//////////////////////////////////////////////////////////////////
// Processes all Windows messages for this window
LRESULT Window::HandleMsg( 
    HWND   hWnd, 
    UINT   uMsg, 
    WPARAM wParam, 
    LPARAM lParam ) noexcept
{
    // Switch on message type
    switch ( uMsg )
    {
        // Prevent manual movement and resizing of the window if fullscreen
        case WM_WINDOWPOSCHANGING:
        {
            if ( fullscreen )
            {
                WINDOWPOS* pWinPos = reinterpret_cast<WINDOWPOS*>( lParam );
                pWinPos->flags |= SWP_NOMOVE | SWP_NOSIZE;
                ShowWindow( hWnd, SW_MAXIMIZE );
            }
            break;
        }
        // Close the window cleanly
        case WM_CLOSE:
        {
            PostQuitMessage( 0 );
            return 0;
        }
    }
    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}
