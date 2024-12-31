#include "User/App.h"

/* ======================================================================================================= */
/*                           [PUBLIC] App                                                                  */
/* ======================================================================================================= */

//////////////////////////////////////////////////////////////////
// [PUBLIC] Sets up the application window
App::App() : wnd( 720, 480, L"Graphics" ) {}

//////////////////////////////////////////////////////////////////
// [PUBLIC] Runs the application loop and returns a termination
//      int upon close; may raise exceptions
int App::Run()
{
    while ( true )
    {
        // Terminate if window processes a quit message
        if ( const auto termination = wnd.ProcessMessages() )
            return termination.value();

        // Update the application logic
        Update();

        // Update the graphics display
        wnd.gfx.Update();
    }
}

//////////////////////////////////////////////////////////////////
// [PRIVATE] Executes all frame logic and updates the window
void App::Update()
{
    wnd.gfx.DrawTriangle( { 200, 200 }, { 300, 400 }, { 350, 150 }, { 0xffffff } );
}
