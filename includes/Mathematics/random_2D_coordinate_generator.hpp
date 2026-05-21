#pragma once

#include <cstdint>

class Random2DCoordinateGenerator
{
	public:
		Random2DCoordinateGenerator(long long seed = 0): _seed(seed) {} 
		~Random2DCoordinateGenerator() = default;

		/**
		 * @brief return the seed of the generator
		 */
		long long seed();

		/**
		 * @brief Generate a pseudo-random number
		 * 
		 */
		long long operator()(const long long& x, const long long& y);

	private:

		/**
		 * @brief Hash the x and y coordinates with the seed value

		 * @return uint64_t the hash value
		 */
		uint64_t hash(const long long& x, const long long& y);

		/**
		 * @brief Mix a hash to avoid patterns
		 * 
		 * @return uint64_t mixed hash value
		 */
		uint64_t mixBits64(uint64_t hash);

		long long _seed;
	


};