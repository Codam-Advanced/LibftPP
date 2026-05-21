#include "Mathematics/random_2D_coordinate_generator.hpp"
#include <cstdint>

uint64_t Random2DCoordinateGenerator::hash(const long long& x, const long long& y)
{
	// extract the seed
	uint64_t hash = static_cast<uint64_t>(_seed);

	// mix seed + coordinates
	hash ^= static_cast<uint64_t>(x) * 0x9E3779B97F4A7C15ULL; // the golden ratio (φ)
	hash ^= static_cast<uint64_t>(y) * 0xC2B2AE3D27D4EB4FULL; // modern hash design (inspired by MurmurHash-style constants)

	hash = mixBits64(hash);

	return hash;
}

uint64_t Random2DCoordinateGenerator::mixBits64(uint64_t hash)
{
	// avalanche mixing
	// we try to switch as many bits as possible with an avalanche (bits flipping because of bits that flipped)

	// we use irregular bit shifting to avoid symmetry with the 64 bit structure
	// the reason we use high bit values is so that we disrupt as many bits as possible
	
	hash ^= (hash >> 30); // shift 30 bits to the right which affects the lower 34 bits
	hash *= 0xBF58476D1CE4E5B9ULL; // avalanche constant
    hash ^= (hash >> 27);
    hash *= 0x94D049BB133111EBULL; // avalanche constant
    hash ^= (hash >> 31);

	// return the value
	return hash;
}

long long Random2DCoordinateGenerator::operator()(const long long& x, const long long& y)
{
	// cast the unsigned long long into a long long
	return static_cast<long long>(hash(x, y));
}

long long Random2DCoordinateGenerator::seed()
{
	return _seed;
}