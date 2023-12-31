#include "Display.h"

////////////////////////////////////////////////////////////
//Stores a graphics reference
Display::Display(Graphics gfx)
	:
	graphics(gfx)
{}

////////////////////////////////////////////////////////////
//Public function that completes all frame logic
void Display::DisplayFrame()
{
	ProcessFrame();
	graphics.Update();
}

////////////////////////////////////////////////////////////
//Any changes to the frame happen within here
void Display::ProcessFrame()
{
	graphics.DrawLine({ 100, 200 }, { 200, 100 }, 0xffffff);
}
