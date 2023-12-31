#include "Graphics.h"

////////////////////////////////////////////////////////////
//Constructs the graphics object and stores necessary data
Graphics::Graphics(HWND& hWindow)
{
	//Get device handle to our window
	hdc = GetDC(hWindow);

	//Get window rectangle and store info about it
	RECT rect;
	GetClientRect(hWindow, &rect);
	clientWidth = rect.right - rect.left;
	clientHeight = rect.bottom - rect.top;
	//clientWidth = 2000;
	//clientHeight = 2000;

	//Allocate our memory for storing pixel values
	memory = VirtualAlloc(
		NULL,													//Location of desired memory (null means we dont care)
		clientWidth * clientHeight * sizeof(unsigned int),		//Amount of memory we want in bytes (num pixels * num bytes per pixel)
		MEM_RESERVE | MEM_COMMIT,								//Type of memory allocation (we want to reserve it and commit to it)
		PAGE_READWRITE											//Memory protection (we will be writing and reading the memory)
	);

	//Initialize values for the bitmap so it can be passed as our new frame each loop

	bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);			//Numer of bytes required by the struct (not including color table)
	bitmap.bmiHeader.biWidth = clientWidth;						//Width of the bitmap in pixels (i.e. the width of our window)
	bitmap.bmiHeader.biHeight = clientHeight;					//Height of the bitmap in pixels (i.e. the height of our window)
	bitmap.bmiHeader.biPlanes = 1;								//Must be set to 1 (says that the data is ordered in memory?)
	bitmap.bmiHeader.biBitCount = sizeof(unsigned int) * 8;		//Number of bits per pixel (num bytes per pixel * num bits in a byte)
	bitmap.bmiHeader.biCompression = BI_RGB;					//Compression type (ours is uncompressed RGB values)

	//Set the screen to the default grey color
	ClearScreen(0x333333);
}

////////////////////////////////////////////////////////////
//Draws a line between two points
void Graphics::DrawLine(Vector<int> pos1, Vector<int> pos2, u32 color)
{
	int x1 = pos1.x;
	int y1 = pos1.y;
	int x2 = pos2.x;
	int y2 = pos2.y;

	int dx = abs(x2 - x1);
	int sx = x1 < x2 ? 1 : -1;
	int dy = -abs(y2 - y1);
	int sy = y1 < y2 ? 1 : -1;
	int error = dx + dy;

	while (true)
	{
		ChangePixel({ x1, y1 }, color);

		if (x1 == x2 && y1 == y2) break;

		int e2 = 2 * error;

		if (e2 >= dy)
		{
			if (x1 == x2) break;

			error += dy;
			x1 += sx;
		}
		if (e2 <= dx)
		{
			if (y1 == y2) break;

			error = error + dx;
			y1 += sy;
		}
	}

}

////////////////////////////////////////////////////////////
//Changes the color of a single pixel
void Graphics::ChangePixel(Vector<int> pos, u32 color)
{
	//Set our pixel variable to the address of the start of our memory block
	u32* pixel = (u32*)memory;

	//Move to the desired coordinate
	pixel += pos.y * clientWidth + pos.x;

	//Change the pixel stored at this coordinate to our passed in color
	*pixel = color;
}

////////////////////////////////////////////////////////////
//Displays the current frame to the screen and resets
void Graphics::Update()
{
	//Method that takes in a device independent bitmap and draws it to the screen
	StretchDIBits(
		hdc,				//Handle to destination (window)
		0,					//Upper left x coordinate of destination (window)
		0,					//Upper left y coordinate of destination (window)
		clientWidth,		//Width of destination (window)
		clientHeight,		//Height of destination (window)
		0,					//Upper left x coordinate of source (bitmap)
		0,					//Upper left y coordinate of source (bitmap)
		clientWidth,		//Width of source (bitmap)
		clientHeight,		//Height of source (bitmap)
		memory,				//Pointer to our allocated location in memory
		&bitmap,			//The bitmap we created and will display to screen
		DIB_RGB_COLORS,		//Tells the function that we are using RGB values
		SRCCOPY				//Directly copy the source to destination, no funny business
	);

	//Set the screen to the default color
	ClearScreen(defaultColor);
}

////////////////////////////////////////////////////////////
//Clears the entire screen with a single color
void Graphics::ClearScreen(u32 color)
{
	//Set our pixel variable to the address of the start of our memory block
	u32* pixel = (u32*)memory;

	//Loop through every pixel in the memory
	for (int i = 0; i < clientWidth * clientHeight; ++i)
	{
		//Set the current pixel to our passed in color and move on to the next
		*pixel++ = color;
	}
}
