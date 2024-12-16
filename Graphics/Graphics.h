#pragma once
#include <Windows.h>
#include "Vec2.h"
#include "Color.h"

using u32 = uint32_t;

class Graphics
{
public:
	//////////////////////////////////////////////////////////////////
	// @brief Constructs the graphics object and stores necessary data
	// 
	// @param hWindow: a Windows window handle
	Graphics(HWND& hWindow);

	//////////////////////////////////////////////////////////////////
	// @brief Draws a rectangle
	//
	// @param corner1, corner2: corners of the rectangle
	// @param color: constant color of the rectangle
	void DrawRectangle(const Vec2<int>& corner1, const Vec2<int>& corner2, const Color& color);

	//////////////////////////////////////////////////////////////////
	// @brief Draws a triangle
	//
	// @param v1, v2, v3: vertices of the triangle
	// @param color: constant color of the trangle
	void DrawTriangle(const Vec2<int>& v1, const Vec2<int>& v2, const Vec2<int>& v3, const Color& color);

	//////////////////////////////////////////////////////////////////
	// @brief Draws a line between two points
	//
	// @param pos1, pos2: points to draw the line between
	// @param color: constant color of the line
	void DrawLine(const Vec2<int>& pos1, const Vec2<int>& pos2, const Color& color);

	//////////////////////////////////////////////////////////////////
	// @brief Changes the color of a single pixel, does NOT check
	// bounds
	// 
	// @param pos: location of the pixel
	// @param color: desired color of the pixel
	void ChangePixel(const Vec2<int>& pos, const Color& color);
	
	//////////////////////////////////////////////////////////////////
	// @brief Displays the current frame to the screen and resets
	void Update();

private:
	//////////////////////////////////////////////////////////////////
	// @brief Clears the entire screen with a single color
	//
	// @param color: color to clear the screen with
	void ClearScreen(const Color& color);

private:
	HDC hdc;
	int clientWidth;
	int clientHeight;
	void* memory;
	BITMAPINFO bitmap;
	Color defaultColor = Color(0x333333);
};