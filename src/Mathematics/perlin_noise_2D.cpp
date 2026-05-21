#include "Mathematics/perlin_noise_2D.hpp"
#include "Mathematics/ivector2.hpp"
#include "Mathematics/random_2D_coordinate_generator.hpp"
#include <cmath>


IVector2<float> PerlinNoise2D::applyFade(IVector2<float> vector)
{
	return IVector2<float> {
		static_cast<float>(6 * std::pow(vector.x, 5) - 15 * std::pow(vector.x, 4) + 10 * std::pow(vector.x, 3)),
		static_cast<float>(6 * std::pow(vector.y, 5) - 15 * std::pow(vector.y, 4) + 10 * std::pow(vector.y, 3)),
	};
}


float PerlinNoise2D::applyLinearInterpolation(float startPosition, float endPosition, float fade)
{
	return startPosition + fade * (endPosition - startPosition);
}

float PerlinNoise2D::sample(float x, float y)
{
	IVector2<float> vector = {x, y};

	IVector2<float> corner0 = {std::floor(x), std::floor(y)};
	IVector2<float> corner1 = {corner0.x + 1, corner0.y + 1};

	// position of the point relative to the box
	IVector2<float> localPosition = vector - corner0;

	IVector2<float> fadeVector = applyFade(localPosition);

	// apply gradients
	IVector2<float> bottomLeft = gradient(corner0);
	IVector2<float> bottomRight = gradient({corner1.x, corner0.y});
	IVector2<float> topLeft = gradient({corner0.x, corner1.y});
	IVector2<float> topRight = gradient(corner1);

	// preform dot calculation on the gradients
	float bottomLeftScalar = bottomLeft.dot(localPosition);
	float bottomRightScalar = bottomRight.dot({localPosition.x - 1, localPosition.y});
	float topLeftScalar = topLeft.dot({localPosition.x, localPosition.y - 1});
	float topRightScalar = topRight.dot({localPosition.x - 1, localPosition.y - 1});

	// apply horizontal interpolation (on the x-axis)
	float bottomInterpolation = applyLinearInterpolation(bottomLeftScalar, bottomRightScalar, fadeVector.x);
	float topInterpolation = applyLinearInterpolation(topLeftScalar, topRightScalar, fadeVector.x);

	// apply vertical interpolation (on the y-axis)
	return applyLinearInterpolation(bottomInterpolation, topInterpolation, fadeVector.y);
}
IVector2<float> PerlinNoise2D::gradient(IVector2<float> vector)
{
	// construct a randomizer
	Random2DCoordinateGenerator randomizer;

	// generate a random number
    uint64_t h = randomizer(vector.x, vector.y);

    // Convert lower bits into [0, 2π]
    float angle = (h & 0xFFFFFFFFULL) * 
                  (2.0f * M_PI / 4294967295.0f);

    return IVector2<float> {
        std::cos(angle),
        std::sin(angle)
    };
}

float PerlinNoise2D::operator()(float x, float y) { return sample(x, y); }