#include "perlin_noise_2D.hpp"

static double fade(const double &t)
{
	return (t * t * t * (t * (t * 6 - 15) + 10));
}

static double lerp(const double &t, const double &a, const double &b)
{
	return (a + t * (b - a));
}

static double grad(const int &hash, const double &x, const double &y)
{
	int h = hash & 7;
	double u = h < 4 ? x : y;
	double v = h < 4 ? y : x;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

PerlinNoise::PerlinNoise()
{
	for (int i=0; i < 256 ; i++)
		p[256+i] = p[i];
}

float PerlinNoise::sample(double x, double y) const
{
	int X = (int)std::floor(x) & 255;
	int Y = (int)std::floor(y) & 255;

	x -= std::floor(x);
	y -= std::floor(y);

	float u = fade(x);
	float v = fade(y);

	int aa = p[p[X] + Y];
	int ab = p[p[X] + Y + 1];
	int ba = p[p[X + 1] + Y];
	int bb = p[p[X + 1] + Y + 1];

	float res = lerp(v,
				lerp(u, grad(aa, x, y), grad(ba, x - 1, y)),
				lerp(u, grad(ab, x, y - 1), grad(bb, x - 1, y - 1)));

	return (res);
}