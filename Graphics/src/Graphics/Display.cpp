#include "Graphics/Display.h"

/* ======================================================================================================= */
/*                           [PUBLIC] Display                                                              */
/* ======================================================================================================= */

//////////////////////////////////////////////////////////////////
// [PUBLIC] Creates a display object storing a graphics reference
Display::Display( Graphics gfx )
    :
    graphics( gfx )
{}

//////////////////////////////////////////////////////////////////
// [PUBLIC] Public function that completes all frame logic
void Display::DisplayFrame()
{
    ProcessFrame();
    graphics.Update();
}

//////////////////////////////////////////////////////////////////
// [PRIVATE] Any changes to the frame happen within here
void Display::ProcessFrame()
{
    graphics.DrawTriangle( { 200, 200 }, { 300, 400 }, { 350, 150 }, { 0xffffff } );
    //graphics.DrawRectangle( { 200, 200 }, { 100, 100 }, { 0xffffff } );
}
