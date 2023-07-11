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
	graphics.DrawLine({ 100, 100 }, { 200, 100 }, 0xffffff);
	graphics.DrawLine({ 100, 100 }, { 200, 150 }, 0xffffff);
	graphics.DrawLine({ 100, 100 }, { 200, 200 }, 0xffffff);
	graphics.DrawLine({ 100, 100 }, { 200, 250 }, 0xffffff);
	graphics.DrawLine({ 100, 100 }, { 200, 300 }, 0xffffff);
	graphics.DrawLine({ 100, 100 }, { 200, 350 }, 0xffffff);
	graphics.DrawLine({ 100, 100 }, { 200, 400 }, 0xffffff);
	graphics.DrawLine({ 100, 100 }, { 200, 450 }, 0xffffff);
}
