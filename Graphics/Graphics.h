#pragma once
#include <Windows.h>
#include <stdint.h>
#include "Vector.h"

typedef uint32_t u32;

class Graphics
{
public:
	Graphics(HWND& hWindow);
	void DrawLine(Vector<int> pos1, Vector<int> pos2, u32 color);
	void ChangePixel(Vector<int> pos, u32 color);
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