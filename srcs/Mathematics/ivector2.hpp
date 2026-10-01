#pragma once

#include <cmath>
template <typename TType>
struct IVector2
{

	TType	x;
	TType	y;

	IVector2() : x(0), y(0) {}
	IVector2(TType x, TType y) : x(x), y(y) {}

	IVector2	operator+(const IVector2& v) const
	{
		return IVector2(x + v.x, y + v.y);
	}
	IVector2	operator-(const IVector2& v) const
	{
		return IVector2(x - v.x, y - v.y);
	}
	IVector2	operator*(const IVector2& v) const
	{
		return IVector2(x * v.x, y * v.y);
	}
	IVector2	operator/(const IVector2& v) const
	{
		return IVector2(x / v.x, y / v.y);
	}

	bool	operator==(const IVector2& v) const
	{
		return (x == v.x && y == v.y);
	}
	bool	operator!=(const IVector2& v) const
	{
		return (x != v.x || y != v.y);
	}

	float	length() const
	{
		return std::sqrt(x * x + y * y);
	}

	IVector2<float>	normalize() const
	{
		float len = length();
		if (len != 0.f)
			return IVector2<float>(x / len, y / len);
		return IVector2<float>(0.f, 0.f);
	}

	float	dot(const IVector2& v) const
	{
		return (x * v.x + y * v.y);
	}

	IVector2	cross(const IVector2& v) const
	{
		return IVector2(0, x * v.y - y * v.x);
	}
};