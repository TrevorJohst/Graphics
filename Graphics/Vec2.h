#pragma once

template <typename T>
struct Vec2
{
public:
	////////////////////////////////////////////////////////////
	//Swaps the values of two vectors
	void Swap(Vec2<T>& rhs)
	{
		//Store temporary values
		T xTemp = x;
		T yTemp = y;

		//Store rhs values into x and y
		x = rhs.x;
		y = rhs.y;

		//Store temporary lhs values into x and y
		rhs.x = xTemp;
		rhs.y = yTemp;
	}
public:
	T x;
	T y;
};