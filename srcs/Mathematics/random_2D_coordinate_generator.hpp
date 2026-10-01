#pragma once

#include <ctime>
#include <cstdint>
//	Based on https://rosettacode.org/wiki/Pseudo-random_numbers/Splitmix64

class Random2D
{
	private:

	uint64_t	_seed;

	static unsigned long long	_splitmix64(unsigned long long x);

	public:

	Random2D(uint64_t seed = 0);

	long long	operator()(const long long& x, const long long& y);
	long long	seed();
	void	reseed(long long seed);
};