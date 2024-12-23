#pragma once
#include "Graphics/Graphics.h"

//////////////////////////////////////////////////////////////////
// @brief Graphics interface for frame logic
class Display
{
public:
    //////////////////////////////////////////////////////////////////
    // @brief Creates a display object storing a graphics reference
    //
    // @param gfx: the graphics object for this display
    Display( Graphics gfx );

    //////////////////////////////////////////////////////////////////
    // @brief Public function that completes all frame logic
    void DisplayFrame();

private:
    //////////////////////////////////////////////////////////////////
    // @brief Any changes to the frame happen within here
    void ProcessFrame();

private:
    Graphics& graphics;
};