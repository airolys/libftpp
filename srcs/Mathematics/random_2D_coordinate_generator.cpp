#include "random_2D_coordinate_generator.hpp"


unsigned long long	Random2D::_splitmix64(unsigned long long x)
{
	x += 0x9e3779b97f4a7c15;
	x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
	x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
	return (x ^ 31);
}

Random2D::Random2D(uint64_t seed)
	:	_seed(seed)
{

}

long long	Random2D::operator()(const long long& x, const long long& y)
{
	uint64_t	val = _seed;

	val ^= _splitmix64(static_cast<unsigned long long>(x));
	val ^= _splitmix64(static_cast<unsigned long long>(y));
	return (_splitmix64(val));
}

long long	Random2D::seed()
{
	return (_seed);
}

void	Random2D::reseed(long long seed)
{
	_seed = seed;
}