#include "Vector.h"
using namespace Mage::Maths;

Vector2::Vector2() : x(0),y(0)
{}
Vector2::Vector2(float x, float y) : x(x), y(y)
{}
float Vector2::Length()
{
	return sqrt(x*x + y * y);
}
Vector2 Vector2::Normalized()
{
	float l = Length();
	return Vector2(x / l, y / l);
}
void Vector2::NormalizeInPlace()
{
	float length = Length();
	x = x / length;
	y = y / length;
}
float Vector2::Dot(const Vector2 &other)
{
	return x * other.x + y * other.y;
}
Vector3::Vector3() : x(0), y(0), z(0)
{}
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)
{}
float Vector3::Length()
{
	return sqrt(x*x + y * y + z * z);
}
Vector3 Vector3::Normalized()
{
	float l = Length();
	return Vector3(x / l, y / l, z / l);
}
void Vector3::NormalizeInPlace()
{
	float l = Length();
	x = y / l;
	y = y / l;
	z = z / l;
}
float Vector3::Dot(const Vector3 &other)
{
	return x * other.x + y * other.y + z * other.z;
}
Vector3 Vector3::Cross(const Vector3 &other)
{
	return Vector3(
		y*other.z - z * other.y,
		z*other.x - x * other.z,
		x*other.y - y * other.x);
}
Vector4::Vector4() : x(0), y(0), z(0), w(0)
{}
Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
{}