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
	graphics.DrawTriangle({ 200, 200 }, { 300, 400 }, { 350, 200 }, 0xffffff);
	//graphics.DrawLine({ 300, 400 }, { 350, 200 }, 0xffffff);
}
