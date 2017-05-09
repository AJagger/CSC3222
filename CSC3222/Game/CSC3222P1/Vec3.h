#pragma once
#include <cmath>

class Vec3
{
public:
	Vec3() { x = 0, y = 0, z = 0; }
	Vec3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}


	//Basic operators
	Vec3 operator+(const Vec3 &add) const
	{
		return Vec3(x + add.x, y + add.y, z + add.z);
	}

	Vec3 operator-(const Vec3 &subtract) const
	{
		return Vec3(x - subtract.x, y - subtract.y, z - subtract.z);
	}

	Vec3 operator-() const
	{
		return Vec3(-x, -y, -z);
	}

	void operator+=(const Vec3  &add)
	{
		x += add.x;
		y += add.y;
		z += add.z;
	}

	void operator-=(const Vec3  &subtract)
	{
		x -= subtract.x;
		y -= subtract.y;
		z -= subtract.z;
	}

	bool operator==(const Vec3 &comparison) const
	{
		return (comparison.x == x && comparison.y == y && comparison.z == z) ? true : false;
	}

	bool operator!=(const Vec3 &comparison) const
	{ 
		return (comparison.x == x && comparison.y == y && comparison.z == z) ? false : true;
	}

	//Vec3 operator*(const float value) const
	//{
	//	return Vec3(x * value, y * value, z * value);
	//}

	//friend Vec3 operator*(const Vec3 &toMultiply, float value)
	//{
	//	return Vec3(toMultiply.x * value, toMultiply.y * value, toMultiply.z * value);
	//}

	friend inline Vec3 operator*(const Vec3 &toMultiply, float value);
	friend inline Vec3 operator*(float value, const Vec3 &toMultiply);

	Vec3 operator*(const Vec3 &multiply) const
	{
		return Vec3(x * multiply.x, y * multiply.y, z * multiply.z);
	}

	 Vec3 operator/(const float value) const
	{
		return Vec3(x / value, y / value, z / value);
	}

	Vec3 operator/(const Vec3 &divide) const
	{
		return Vec3(x / divide.x, y / divide.y, z / divide.z);
	}



	//Vector specific methods
	float dot(const Vec3 &vector) const
	{
		return x * vector.x + y * vector.y + z * vector.z;
	}

	Vec3 cross(const Vec3 &vector) const
	{
		return Vec3(y * vector.z - z * vector.y, z * vector.x - x * vector.z, x * vector.y - y * vector.x);
	}

	float length() const
	{
		return sqrt((x*x) + (y*y) + (z*z));
	}

	Vec3 normalise() const
	{
		float length = this->length();
		if(length != 0.0f)
		{
			length = 1.0f / length;
			return Vec3(x*length, y*length, z*length);
		}
		else
		{
			return Vec3(x, y, z);
		}
	}


	//Implementation specific methods

	//When using vector3 to store position vector for a 2D environment (allowing the use of z coordinate to place items infront of others for rendering purposes)
	float MagnitudeXY()
	{
		return sqrt((x*x) + (y*y));
	}


	//Components
	float x, y, z;

};

inline Vec3 operator*(const Vec3 &toMultiply, float value)
{
	return Vec3(toMultiply.x * value, toMultiply.y * value, toMultiply.z * value);
}

inline Vec3 operator*(float value, const Vec3 &toMultiply)
{
	return Vec3(toMultiply.x * value, toMultiply.y * value, toMultiply.z * value);
}
