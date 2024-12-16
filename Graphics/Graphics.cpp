#include <new>
#include <cassert>
#include "Graphics.h"

//////////////////////////////////////////////////////////////////
// Constructs the graphics object and stores necessary data
Graphics::Graphics(HWND& hWindow)
{
    // Get device handle to our window
    hdc = GetDC(hWindow);

    // Get window rectangle and store info about it
    RECT rect;
    GetClientRect(hWindow, &rect);
    clientWidth = rect.right - rect.left;
    clientHeight = rect.bottom - rect.top;

    // Allocate our memory for storing pixel values
    memory = VirtualAlloc(
        NULL,													// Location of desired memory (null means we dont care)
        static_cast<SIZE_T>(clientWidth * clientHeight) 
            * sizeof(u32),		                                // Amount of memory we want in bytes (num pixels * num bytes per pixel)
        MEM_RESERVE | MEM_COMMIT,								// Type of memory allocation (we want to reserve it and commit to it)
        PAGE_READWRITE											// Memory protection (we will be writing and reading the memory)
    );

    // Throw an error if we did not receive memory
    if (!memory) 
        throw std::bad_alloc();

    // Initialize values for the bitmap so it can be passed as our new frame each loop
    bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);			// Number of bytes required by the struct (not including color table)
    bitmap.bmiHeader.biWidth = clientWidth;						// Width of the bitmap in pixels (i.e. the width of our window)
    bitmap.bmiHeader.biHeight = clientHeight;					// Height of the bitmap in pixels (i.e. the height of our window)
    bitmap.bmiHeader.biPlanes = 1;								// Must be set to 1 (says that the data is ordered in memory?)
    bitmap.bmiHeader.biBitCount = sizeof(unsigned int) * 8;		// Number of bits per pixel (num bytes per pixel * num bits in a byte)
    bitmap.bmiHeader.biCompression = BI_RGB;					// Compression type (ours is uncompressed RGB values)

    // Set the screen to the default grey color
    ClearScreen(0x333333);
}

//////////////////////////////////////////////////////////////////
// Draws a rectangle
void Graphics::DrawRectangle(const Vec2<int>& corner1, const Vec2<int>& corner2, const Color& color)
{

}

//////////////////////////////////////////////////////////////////
// Draws a triangle
void Graphics::DrawTriangle(const Vec2<int>& v1, const Vec2<int>& v2, const Vec2<int>& v3, const Color& color)
{
    // Initialize variables
    Vec2<int> top = v1, middle = v2, bottom = v3;

    // Order all of the verticies in descending order
    if (top.y < middle.y)
        top.Swap(middle);

    if (middle.y < bottom.y)
    {
        middle.Swap(bottom);

        if (top.y < middle.y)
            top.Swap(middle);
    }

    // Handle coincident points edge case
    int dx = top.x - bottom.x;
    int dy = top.y - bottom.y;

    if (dy == 0)
    {
        int left = min(min(top.x, middle.x), bottom.x);
        int right = max(max(top.x, middle.x), bottom.x);

        DrawLine({ left, middle.y }, { right, middle.y }, color);
        return;
    }

    // Find new middle vertex
    int newX = static_cast<int>(static_cast<float>(dx) / static_cast<float>(dy) * (middle.y - top.y) + top.x);

    // Order middle vertices
    Vec2<int> middleLeft = { newX, middle.y }, middleRight = middle;
    if (middleLeft.x > middleRight.x) middleLeft.Swap(middleRight);


    //// Draw flat bottom triangle
    int lx1 = middleLeft.x, ly1 = middleLeft.y;
    int lx2 = top.x, ly2 = top.y;

    int ldx = abs(lx2 - lx1);
    int lxStep = lx1 < lx2 ? 1 : -1;
    int ldy = -abs(ly2 - ly1);
    int lyStep = ly1 < ly2 ? 1 : -1;
    int lerror = ldx + ldy;

    int rx1 = middleRight.x, ry1 = middleRight.y;
    int rx2 = top.x, ry2 = top.y;

    int rdx = abs(rx2 - rx1);
    int rxStep = rx1 < rx2 ? 1 : -1;
    int rdy = -abs(ry2 - ry1);
    int ryStep = ry1 < ry2 ? 1 : -1;
    int rerror = rdx + rdy;

    //  Draw the left line
    while (true)
    {
        if (lx1 == lx2 && ly1 == ly2) break;

        int le2 = 2 * lerror;

        // If we are in the negative half-plane
        if (le2 >= ldy)
        {
            if (lx1 == lx2) break;

            // Step along x and accumulate y error
            lerror += ldy;
            lx1 += lxStep;
        }
        // If we are in the positive half-plane
        if (le2 <= ldx)
        {
            if (ly1 == ly2) break;

            // Start drawing the right line
            while (true)
            {
                // Double the error for comparison (avoids floating points)
                int re2 = 2 * rerror;

                // If we are in the negative half-plane
                if (re2 >= rdy)
                {
                    // Step along x and accumulate y error
                    rerror += rdy;
                    rx1 += rxStep;
                }
                // If we are in the positive half-plane
                if (re2 <= rdx)
                {
                    // Draw the horizontal line between the left and right line
                    for (int x = lx1; x <= rx1; x++)
                        ChangePixel({ x, ly1 }, color);

                    // Step along y and accumulate x error
                    rerror = rerror + rdx;
                    ry1 += ryStep;

                    break;
                }
            }

            // Step along y and accumulate x error
            lerror = lerror + ldx;
            ly1 += lyStep;
        }
    }


    //// Draw flat top triangle
    lx1 = middleLeft.x, ly1 = middleLeft.y;
    lx2 = bottom.x, ly2 = bottom.y;

    ldx = abs(lx2 - lx1);
    lxStep = lx1 < lx2 ? 1 : -1;
    ldy = -abs(ly2 - ly1);
    lyStep = ly1 < ly2 ? 1 : -1;
    lerror = ldx + ldy;

    rx1 = middleRight.x, ry1 = middleRight.y;
    rx2 = bottom.x, ry2 = bottom.y;

    rdx = abs(rx2 - rx1);
    rxStep = rx1 < rx2 ? 1 : -1;
    rdy = -abs(ry2 - ry1);
    ryStep = ry1 < ry2 ? 1 : -1;
    rerror = rdx + rdy;

    // Draw the left line
    while (true)
    {
        if (lx1 == lx2 && ly1 == ly2) break;

        int le2 = 2 * lerror;

        // If we are in the negative half-plane
        if (le2 >= ldy)
        {
            if (lx1 == lx2) break;

            // Step along x and accumulate y error
            lerror += ldy;
            lx1 += lxStep;
        }
        // If we are in the positive half-plane
        if (le2 <= ldx)
        {
            if (ly1 == ly2) break;

            // Start drawing the right line
            while (true)
            {
                // Double the error for comparison (avoids floating points)
                int re2 = 2 * rerror;

                // If we are in the negative half-plane
                if (re2 >= rdy)
                {
                    // Step along x and accumulate y error
                    rerror += rdy;
                    rx1 += rxStep;
                }
                // If we are in the positive half-plane
                if (re2 <= rdx)
                {
                    // Draw the horizontal line between the left and right line
                    for (int x = lx1; x <= rx1; x++)
                        ChangePixel({ x, ly1 }, color);

                    // Step along y and accumulate x error
                    rerror = rerror + rdx;
                    ry1 += ryStep;

                    break;
                }
            }

            // Step along y and accumulate x error
            lerror = lerror + ldx;
            ly1 += lyStep;
        }
    }
}

//////////////////////////////////////////////////////////////////
// Draws a line between two points
void Graphics::DrawLine(const Vec2<int>& pos1, const Vec2<int>& pos2, const Color& color)
{
    // Unpack coordinates
    int x1 = pos1.x, y1 = pos1.y;
    int x2 = pos2.x, y2 = pos2.y;

    // Find differences and steps based on incrementing or decrementing
    int dx = abs(x2 - x1);
    int xStep = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1);
    int yStep = y1 < y2 ? 1 : -1;

    // Initialize error
    int error = dx + dy;

    while (true)
    {
        // Draw current pixel
        ChangePixel({ x1, y1 }, color);

        // End if we reach the other point
        if (x1 == x2 && y1 == y2) break;

        // Double the error for comparison (avoids floating points)
        int e2 = 2 * error;

        // If we are in the negative half-plane
        if (e2 >= dy)
        {
            if (x1 == x2) break;

            // Step along x and accumulate y error
            error += dy;
            x1 += xStep;
        }
        // If we are in the positive half-plane
        if (e2 <= dx)
        {
            if (y1 == y2) break;

            // Step along y and accumulate x error
            error = error + dx;
            y1 += yStep;
        }
    }
}

//////////////////////////////////////////////////////////////////
// Changes the color of a single pixel
void Graphics::ChangePixel(const Vec2<int>& pos, const Color& color)
{
    assert(pos.x >= 0 && pos.x < clientWidth);
    assert(pos.y >= 0 && pos.y < clientHeight);

    // Set our pixel variable to the address of the start of our memory block
    u32* pixel = (u32*)memory;

    // Move to the desired coordinate
    pixel += pos.y * clientWidth + pos.x;

    // Change the pixel stored at this coordinate to our passed in color
    *pixel = color.hex;
}

//////////////////////////////////////////////////////////////////
// Displays the current frame to the screen and resets
void Graphics::Update()
{
    // Method that takes in a device independent bitmap and draws it to the screen
    StretchDIBits(
        hdc,				// Handle to destination (window)
        0,					// Upper left x coordinate of destination (window)
        0,					// Upper left y coordinate of destination (window)
        clientWidth,		// Width of destination (window)
        clientHeight,		// Height of destination (window)
        0,					// Upper left x coordinate of source (bitmap)
        0,					// Upper left y coordinate of source (bitmap)
        clientWidth,		// Width of source (bitmap)
        clientHeight,		// Height of source (bitmap)
        memory,				// Pointer to our allocated location in memory
        &bitmap,			// The bitmap we created and will display to screen
        DIB_RGB_COLORS,		// Tells the function that we are using RGB values
        SRCCOPY				// Directly copy the source to destination, no funny business
    );

    // Set the screen to the default color
    ClearScreen(defaultColor);
}

//////////////////////////////////////////////////////////////////
// Clears the entire screen with a single color
void Graphics::ClearScreen(const Color& color)
{
    // Set our pixel variable to the address of the start of our memory block
    u32* pixel = (u32*)memory;

    // Loop through every pixel in the memory
    for (int i = 0; i < clientWidth * clientHeight; ++i)
    {
        // Set the current pixel to our passed in color and move on to the next
        *pixel++ = color.hex;
    }
}
