#pragma once

#include <cmath>

template<typename TType>
struct IVector2 {

	// Arithmetic operators
	IVector2<TType> operator+(const IVector2<TType>& other)
	{
		return IVector2 {
			x + other.x,
			y + other.y,
		};
	}
	IVector2<TType> operator-(const IVector2<TType>& other)
	{
		return IVector2 {
			x - other.x,
			y - other.y,
		};
	}
	IVector2<TType> operator*(const IVector2<TType>& other)
	{
		return IVector2 {
			x * other.x,
			y * other.y,
		};
	}
	IVector2<TType> operator/(const IVector2<TType>& other)
	{
		return IVector2 {
			x / other.x,
			y / other.y,
		};
	}

	// comparison operators
	bool operator==(const IVector2<TType>& other)
	{
		return x == other.x && y == other.y;
	}
	bool operator!=(const IVector2<TType>& other)
	{
		return x != other.x || y != other.y;
	}

	// return the norm/length of the vector
	float length()
	{
		// calculate the length (magnitude) of the vector
		return std::sqrt((x * x) + (y * y));
	}

	// returns the dot product of the 2D vector
	float dot(const IVector2<TType>& other)
	{
		return (x * other.x) + (y * other.y);
	}

	// returns the normalized version of the 2D vector
	IVector2<float> normalize()
	{
		// calculate the length of the vector
		float magnitude = length();

		// divide the components by the magnitude of the vector 
		return IVector2<float>{
			x / magnitude,
			y / magnitude		
		};
	}

	// returns the cross product of the 2D vector
	IVector2<TType> cross()
	{
		return IVector2 {-y, x};
	}

	// coordinates
	TType x;
	TType y;
};