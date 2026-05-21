#pragma once

#include <cmath>

template<typename TType>
struct IVector3 {

	// Arithmetic operators
	IVector3<TType> operator+(const IVector3<TType>& other)
	{
		return IVector3 {
			x + other.x,
			y + other.y,
			z + other.z,
		};
	}
	IVector3<TType> operator-(const IVector3<TType>& other)
	{
		return IVector3 {
			x - other.x,
			y - other.y,
			z - other.z,
		};
	}
	IVector3<TType> operator*(const IVector3<TType>& other)
	{
		return IVector3 {
			x * other.x,
			y * other.y,
			z * other.z,
		};
	}
	IVector3<TType> operator/(const IVector3<TType>& other)
	{
		return IVector3 {
			x / other.x,
			y / other.y,
			z / other.z,
		};
	}

	// comparison operators
	bool operator==(const IVector3<TType>& other)
	{
		return x == other.x && y == other.y && z == other.z;
	}
	bool operator!=(const IVector3<TType>& other)
	{
		return x != other.x || y != other.y || z != other.z;
	}

	// return the norm/length of the vector
	float length()
	{
		// calculate the length (magnitude) of the vector
		return std::sqrt((x * x) + (y * y) + (z * z));
	}

	// returns the dot product of the 2D vector
	float dot(const IVector3<TType>& other)
	{
		return (x * other.x) + (y * other.y) + (z * other.z);
	}

	// returns the normalized version of the 2D vector
	IVector3<float> normalize()
	{
		// calculate the length of the vector
		float magnitude = length();

		// divide the components by the magnitude of the vector 
		return IVector3<float>{
			x / magnitude,
			y / magnitude,
			z / magnitude
		};
	}

	// returns the cross product of the 2D vector
	IVector3<TType> cross(const IVector3<TType>& other)
	{
		return IVector3 {
			(y * other.z) - (z * other.y),
			(z * other.x) - (x * other.z),
			(x * other.y) - (y * other.x)
		};
	}

	// coordinates
	TType x;
	TType y;
	TType z;
};