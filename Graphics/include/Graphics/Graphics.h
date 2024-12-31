#pragma once
#include "Windows/Win.h"
#include "Utility/Vec2.h"
#include "Utility/Color.h"

//////////////////////////////////////////////////////////////////
// @brief Graphics pipeline for a given window
class Graphics
{
public:
    // TEMPORARY FIX UNTIL GRAPHICS REFACTOR
    Graphics() = default;

    //////////////////////////////////////////////////////////////////
    // @brief Constructs the graphics object and stores necessary data
    // 
    // @param hWindow: a Windows window handle
    Graphics( const HWND& hWindow );


    //////////////////////////////////////////////////////////////////
    // @brief Draws a rectangle
    //
    // @param corner1: first corner of the rectangle
    // @param corner2: second corner of the rectangle
    // @param color: constant color of the rectangle
    void DrawRectangle(
        const Vec2<int>& corner1,
        const Vec2<int>& corner2,
        const Color&     color );

    //////////////////////////////////////////////////////////////////
    // @brief Draws a triangle
    //
    // @param v1: first vertex of the triangle
    // @param v2: second vertex of the triangle
    // @param v3: third vertex of the triangle
    // @param color: constant color of the trangle
    void DrawTriangle(
        const Vec2<int>& v1,
        const Vec2<int>& v2,
        const Vec2<int>& v3,
        const Color&     color );

    //////////////////////////////////////////////////////////////////
    // @brief Draws a line between two points
    //
    // @param pos1: starting point of line
    // @param pos2: end point of line
    // @param color: constant color of the line
    void DrawLine(
        const Vec2<int>& pos1,
        const Vec2<int>& pos2,
        const Color&     color );

    //////////////////////////////////////////////////////////////////
    // @brief Changes the color of a single pixel, does NOT check
    // bounds
    // 
    // @param pos: location of the pixel
    // @param color: desired color of the pixel
    void ChangePixel(
        const Vec2<int>& pos,
        const Color&     color );


    //////////////////////////////////////////////////////////////////
    // @brief Displays the current frame to the screen and resets
    void Update();

private:
    //////////////////////////////////////////////////////////////////
    // @brief Clears the entire screen with a single color
    //
    // @param color: color to clear the screen with
    void ClearScreen( const Color& color );

private:
    HDC hdc;
    int clientWidth;
    int clientHeight;
    void* memory;
    BITMAPINFO bitmap;
    Color defaultColor = Color( 0x333333 );
};