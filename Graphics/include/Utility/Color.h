#pragma once
#include <stdint.h>

//////////////////////////////////////////////////////////////////
// @brief RGB color represented in 32 bits
struct Color
{
public:
    //////////////////////////////////////////////////////////////////
    // @brief Constructs the color object with a hex code
    //
    // @param hex: hex value of the color
    Color( uint32_t hex ) : hex( hex ) {}

    //////////////////////////////////////////////////////////////////
    // @brief Constructs the color object with rgb values
    //
    // @param r: red channel [0-255]
    // @param g: green channel [0-255]
    // @param b: blue channel [0-255]
    Color( uint8_t r, uint8_t g, uint8_t b ) : hex( ( r << 16 ) | ( g << 8 ) | b ) {}

public:
    uint32_t hex;
};