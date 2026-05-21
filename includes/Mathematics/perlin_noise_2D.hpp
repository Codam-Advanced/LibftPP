#pragma once

#include "Mathematics/ivector2.hpp"
class PerlinNoise2D
{
	public:
		float sample(float x, float y);
		
		float operator()(float x, float y);

	private:

		IVector2<float> applyFade(IVector2<float> vector);

		float applyLinearInterpolation(float startPosition, float endPosition, float fade);
		IVector2<float> gradient(IVector2<float> vector);
};