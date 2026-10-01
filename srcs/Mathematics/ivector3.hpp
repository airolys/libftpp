#pragma once

#include <cmath>

template <typename TType>
struct IVector3
{

	TType	x;
	TType	y;
	TType	z;

	IVector3() : x(0), y(0), z(0) {}
	IVector3(TType x, TType y, TType z) : x(x), y(y), z(z) {}

	IVector3	operator+(const IVector3& v) const
	{
		return IVector3(x + v.x, y + v.y, z + v.z);
	}
	IVector3	operator-(const IVector3& v) const
	{
		return IVector3(x - v.x, y - v.y, z - v.z);
	}
	IVector3	operator*(const IVector3& v) const
	{
		return IVector3(x * v.x, y * v.y, z * v.z);
	}
	IVector3	operator/(const IVector3& v) const
	{
		return IVector3(x / v.x, y / v.y, z / v.z);
	}

	bool	operator==(const IVector3& v) const
	{
		return (x == v.x && y == v.y && z == v.z);
	}
	bool	operator!=(const IVector3& v) const
	{
		return (x != v.x || y != v.y || z != v.z);
	}

	float			length() const
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	IVector3<float>	normalize() const
	{
		float len = length();
		if (len != 0.f)
			return IVector3<float>(x / len, y / len, z / len);
		return IVector3<float>(0.f, 0.f, 0.f);
	}

	float		dot(const IVector3& v) const
	{
		return (x * v.x + y * v.y + z * v.z);
	}

	IVector3	cross(const IVector3& v) const
	{
		return	(IVector3<TType>(y * v.z - z * v.y,
								z * v.x - x * v.z,
								x * v.y - y * v.x));
	}
};