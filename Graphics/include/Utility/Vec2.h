#pragma once

//////////////////////////////////////////////////////////////////
// @brief 2 dimensional vector with x and y
template <typename T>
struct Vec2
{
public:
    //////////////////////////////////////////////////////////////////
    // @brief Constructs a 2D vector 
    //
    // @param x: x coordinate of the vector
    // @param y: y coordinate of the vector
    Vec2( T x, T y ) : x( x ), y( y ) {}

    //////////////////////////////////////////////////////////////////
    // @brief Swaps the values of two vectors
    //
    // @param rhs: vector to swap with
    void Swap( Vec2<T>& rhs )
    {
        // Swap x
        T temp = x;
        x = rhs.x;
        rhs.x = temp;

        // Swap y
        temp = y;
        y = rhs.y;
        rhs.y = temp;
    }

public:
    T x;
    T y;
};