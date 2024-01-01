#pragma once
#include <Windows.h>
#include <stdint.h>
#include "Vec2.h"

typedef uint32_t u32;

class Graphics
{
public:
	Graphics(HWND& hWindow);
	void DrawTriangle(Vec2<int> v1, Vec2<int> v2, Vec2<int> v3, u32 color);
	void DrawLine(Vec2<int> pos1, Vec2<int> pos2, u32 color);
	void ChangePixel(Vec2<int> pos, u32 color);
	void Update();
private:
	void ClearScreen(u32 color);
private:
	HDC hdc;
	int clientWidth;
	int clientHeight;
	void* memory;
	BITMAPINFO bitmap;
	u32 defaultColor = 0x333333;
};