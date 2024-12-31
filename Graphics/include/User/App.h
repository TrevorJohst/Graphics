#pragma once
#include "Windows/Window.h"

//////////////////////////////////////////////////////////////////
// @brief A user application that can interface with a window
class App
{
public:
    //////////////////////////////////////////////////////////////////
    // @brief Sets up the application window
    App();

    //////////////////////////////////////////////////////////////////
    // @brief Runs the application loop and returns a termination int
    //      upon close; may raise exceptions
    int Run();

private:
    //////////////////////////////////////////////////////////////////
    // @brief Executes all frame logic and updates the window
    void Update();

private:
    Window wnd;
};