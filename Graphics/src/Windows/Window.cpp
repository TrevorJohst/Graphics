#include "Windows/Window.h"
#include <sstream>
#include <cassert>

/* ======================================================================================================= */
/*                           [PUBLIC] Window::Exception                                                    */
/* ======================================================================================================= */

//////////////////////////////////////////////////////////////////
// [PUBLIC] Constructs a custom Window::Exception
Window::Exception::Exception(
    int         line,
    const char* file,
    HRESULT     hr ) noexcept
    :
    GraphicsException( line, file ),
    hr( hr )
{}

//////////////////////////////////////////////////////////////////
// [PUBLIC] Human readable error string recovered from exception
const char* Window::Exception::what() const noexcept
{
    // Format the error string and store in buffer
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] " << GetErrorCode() << std::endl
        << "[Description] " << GetErrorString() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();

    // Return pointer to persistent buffer string
    return whatBuffer.c_str();
}

//////////////////////////////////////////////////////////////////
// [PUBLIC] Returns Windows Error type of exception
const char* Window::Exception::GetType() const noexcept { return "Window Exception"; }

//////////////////////////////////////////////////////////////////
// [PUBLIC] Returns the Windows HRESULT error code of this exception
HRESULT Window::Exception::GetErrorCode() const noexcept { return hr; }

//////////////////////////////////////////////////////////////////
// [PUBLIC] Returns the string of the error code for this 
//          exception
std::string Window::Exception::GetErrorString() const noexcept { return TranslateErrorCode( hr ); }

//////////////////////////////////////////////////////////////////
// [PUBLIC] Translates a Windows HRESULT error code into a string
std::string Window::Exception::TranslateErrorCode( HRESULT hr )
{
    // Translate the message using the Windows API
    char* pMsgBuf = nullptr;
    DWORD nMsgLen = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,          // Format flags
        nullptr,                                // Message definition
        hr,                                     // Error code
        0,                                      // Language (selects based on device settings)
        reinterpret_cast<LPSTR>( &pMsgBuf ),    // Pointer to our pointer, redirected to filled buffer
        0,                                      // Output buffer size (set automatically by flag)
        nullptr                                 // Optional arguments
    );

    // Invalid error code, buffer was not filled
    if ( nMsgLen == 0 )
        return "Unknown error code";

    // Free the message buffer and return the formatted error string
    std::string errorString = pMsgBuf;
    LocalFree( pMsgBuf );
    return errorString;
}

/* ======================================================================================================= */
/*                           [PRIVATE] Window::WindowClass                                                 */
/* ======================================================================================================= */

// wndClass singleton declaration
Window::WindowClass Window::WindowClass::wndClass;

//////////////////////////////////////////////////////////////////
// [PUBLIC] Access the window's name
LPCWSTR Window::WindowClass::GetName() noexcept { return wndClassName; }

//////////////////////////////////////////////////////////////////
// [PUBLIC] Access the window's instance handle
HINSTANCE Window::WindowClass::GetInstance() noexcept { return wndClass.hInst; }

//////////////////////////////////////////////////////////////////
// [PRIVATE] Constructs a single window 
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
// [PRIVATE] Destroys a window on exit 
Window::WindowClass::~WindowClass() { UnregisterClass( wndClassName, GetInstance() ); }

/* ======================================================================================================= */
/*                           [PUBLIC] Window                                                               */
/* ======================================================================================================= */

//////////////////////////////////////////////////////////////////
// [PUBLIC] Creates a single window with desired parameters
Window::Window( 
    int            clientWidth, 
    int            clientHeight,
    const wchar_t* name,
    bool           fullscreen )
    :
    fullscreen(fullscreen)
{
    assert( clientWidth >= 0 && clientHeight >= 0 );

    // Hard coded window style
    const auto STYLE = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

    // Calculate full window sized based on desired client size
    RECT wr = {};
    wr.left = 0;
    wr.right = clientWidth;
    wr.top = 0;
    wr.bottom = clientHeight;

    // Throw error if window adjustment failes
    if ( AdjustWindowRect( &wr, STYLE, FALSE ) == FALSE )
        throw WND_LAST_EXCEPT();

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

    // Window creation failed
    if ( hWnd == nullptr )
        throw WND_LAST_EXCEPT();

    // Show the window
    ShowWindow( hWnd, SW_SHOWDEFAULT );
}

//////////////////////////////////////////////////////////////////
// [PUBLIC] Destroys the window freeing the instance
Window::~Window() { DestroyWindow( hWnd ); }

//////////////////////////////////////////////////////////////////
// [PUBLIC] Access the window's handle
HWND Window::GetHandle() noexcept { return hWnd; }

//////////////////////////////////////////////////////////////////
// [PRIVATE] Sets up message handling at time of window creation
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
// [PRIVATE] Passes message handling to HandleMsg at runtime
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
// [PRIVATE] Processes all Windows messages for this window
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
        // Clear key states when window loses focus
        case WM_KILLFOCUS:
        {
            kbd.ClearState();
            break;
        }
        // Key pressed down event
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            // 30th bit is 1 if key was already down
            if ( !( lParam & ( 0b1 << 30 ) ) || kbd.AutorepeatIsEnabled() )
            {
                kbd.OnKeyPressed( static_cast<unsigned char>( wParam ) );
            }
            break;
        }
        // Key released event
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            kbd.OnKeyReleased( static_cast<unsigned char>( wParam ) );
            break;
        }
        // Char event
        case WM_CHAR:
        {
            kbd.OnChar( static_cast<unsigned char>( wParam ) );
            break;
        }
    }
    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}
