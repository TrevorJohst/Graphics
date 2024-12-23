#pragma once
#include "Windows/Win.h"
#include "Windows/Resource.h"
#include "Utility/GraphicsException.h"

//////////////////////////////////////////////////////////////////
// @brief A single encapsulated window
class Window
{
public:
    //////////////////////////////////////////////////////////////////
    // @brief Custom Windows exceptions with additional information
    class Exception : public GraphicsException
    {
    public:
        //////////////////////////////////////////////////////////////////
        // @brief Constructs a custom Window::Exception
        //
        // @param line, file: where the exception is thrown from
        // @param hr: the Windows HRESULT error code for this exception
        Exception( 
            int         line, 
            const char* file, 
            HRESULT     hr ) noexcept;

        //////////////////////////////////////////////////////////////////
        // @brief Human readable error string recovered from exception
        const char* what() const noexcept override;

        //////////////////////////////////////////////////////////////////
        // @brief Returns Windows Error type of exception
        virtual const char* GetType() const noexcept override;

        //////////////////////////////////////////////////////////////////
        // @brief Translates a Windows HRESULT error code into a string
        //
        // @param hr: the Windows HRESULT error code to be translated
        // @return a string representation of the translated code
        static std::string TranslateErrorCode( HRESULT hr );

        //////////////////////////////////////////////////////////////////
        // @brief Returns the Windows HRESULT error code of this exception
        HRESULT GetErrorCode() const noexcept;

        //////////////////////////////////////////////////////////////////
        // @brief Returns the string of the error code for this exception
        std::string GetErrorString() const noexcept;

    private:
        HRESULT hr;
    };

private:
    //////////////////////////////////////////////////////////////////
    // @brief A singleton for managing instantitation and cleanup
    class WindowClass
    {
    public:
        //////////////////////////////////////////////////////////////////
        // @brief Returns the window's name as a LPCWSTR
        static LPCWSTR GetName() noexcept;

        //////////////////////////////////////////////////////////////////
        // @brief Returns the window's instance handle
        static HINSTANCE GetInstance() noexcept;

        //////////////////////////////////////////////////////////////////
        // @brief Copy constructor is deleted to enforce singleton
        WindowClass( const WindowClass& ) = delete;

        //////////////////////////////////////////////////////////////////
        // @brief Assignment operator is deleted to enforce singleton
        WindowClass& operator=( const WindowClass& ) = delete;

    private:
        //////////////////////////////////////////////////////////////////
        // @brief Constructs a single window 
        WindowClass() noexcept;

        //////////////////////////////////////////////////////////////////
        // @brief Destroys a window on exit 
        ~WindowClass();

    private:
        static constexpr LPCWSTR wndClassName = L"Graphics Window";
        static WindowClass wndClass;
        HINSTANCE hInst;
    };

public:
    //////////////////////////////////////////////////////////////////
    // @brief Creates a single window with desired parameters
    //
    // @param clientWidth, clientHeight: width and height of the
    //      client portion of the window
    // @param name: display name of the window
    // @param fullscreen: if the window should be forced fullscreen,
    //      ignores clientWidth clientHeight if true
    Window( 
        int            clientWidth,
        int            clientHeight,
        const wchar_t* name,
        bool           fullscreen = false);

    //////////////////////////////////////////////////////////////////
    // @brief Destroys the window freeing the instance
    ~Window();

    //////////////////////////////////////////////////////////////////
    // @brief Access the window's handle
    //
    // @return the handle to the window
    HWND GetHandle() noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Copy constructor is deleted to enforce singleton
    Window( const Window& ) = delete;

    //////////////////////////////////////////////////////////////////
    // @brief Assignment operator is deleted to enforce singleton
    Window& operator=( const Window& ) = delete;

private:
    //////////////////////////////////////////////////////////////////
    // @brief Sets up message handling at time of window creation
    //
    // @param hwnd, uMsg, wParam, lParam: default WindowProc params
    // @return default success result
    static LRESULT CALLBACK HandleMsgSetup(
        HWND   hWnd,
        UINT   uMsg,
        WPARAM wParam,
        LPARAM lParam ) noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Passes message handling to HandleMsg at runtime
    //
    // @param hwnd, uMsg, wParam, lParam: default WindowProc params
    // @return default success result
    static LRESULT CALLBACK HandleMsgThunk(
        HWND   hWnd,
        UINT   uMsg,
        WPARAM wParam,
        LPARAM lParam ) noexcept;

    //////////////////////////////////////////////////////////////////
    // @brief Processes all Windows messages for this window
    //
    // @param hwnd, uMsg, wParam, lParam: default WindowProc params
    // @return default success result
    LRESULT HandleMsg(
        HWND   hWnd,
        UINT   uMsg,
        WPARAM wParam,
        LPARAM lParam ) noexcept;

private:
    int width;
    int height;
    HWND hWnd;
    bool fullscreen;
};

// Error macros
#define WND_EXCEPT( hr ) Window::Exception( __LINE__, __FILE__, hr )
#define WND_LAST_EXCEPT() Window::Exception( __LINE__, __FILE__, GetLastError() )