#pragma once
#include "Graphics.h"

class Display
{
public:
	Display(Graphics gfx);
	void DisplayFrame();
private:
	void ProcessFrame();
private:
	Graphics& graphics;
};