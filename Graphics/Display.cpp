#include "Display.h"

//////////////////////////////////////////////////////////////////
// Creates a display object storing a graphics reference
Display::Display(Graphics gfx)
	:
	graphics(gfx)
{}

//////////////////////////////////////////////////////////////////
// Public function that completes all frame logic
void Display::DisplayFrame()
{
	ProcessFrame();
	graphics.Update();
}

//////////////////////////////////////////////////////////////////
// Any changes to the frame happen within here
void Display::ProcessFrame()
{
	graphics.DrawTriangle({ 200, 200 }, { 300, 400 }, { 350, 150 }, { 0xffffff });
}
