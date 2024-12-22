#pragma once

template <typename T>
struct Vec2
{
public:
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