#pragma once
#include "Windows/Win.h"
#include "Windows/Resource.h"

//////////////////////////////////////////////////////////////////
// @brief A single encapsulated window
class Window
{
private:
    //////////////////////////////////////////////////////////////////
    // @brief A singleton for managing instantitation and cleanup
    class WindowClass
    {
    public:
        //////////////////////////////////////////////////////////////////
        // @brief Access the window's name
        //
        // @return the name of the window in the form of a LPCWSTR
        static LPCWSTR GetName() noexcept;

        //////////////////////////////////////////////////////////////////
        // @brief Access the window's instance handle
        //
        // @return the handle to the instance
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
        bool           fullscreen = false) noexcept;

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