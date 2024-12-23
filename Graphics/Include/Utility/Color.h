#pragma once
#include <stdint.h>

using u32 = uint32_t;
using u8 = uint8_t;

//////////////////////////////////////////////////////////////////
// @brief RGB color represented in 32 bits
struct Color
{
public:
    //////////////////////////////////////////////////////////////////
    // @brief Constructs the color object with a hex code
    //
    // @param hex: hex value of the color
    Color( u32 hex ) : hex( hex ) {}

    //////////////////////////////////////////////////////////////////
    // @brief Constructs the color object with rgb values
    //
    // @param r, g, b: single byte red, green, and blue channels
    Color( u8 r, u8 g, u8 b ) : hex( ( r << 16 ) | ( g << 8 ) | b ) {}

public:
    u32 hex;
};