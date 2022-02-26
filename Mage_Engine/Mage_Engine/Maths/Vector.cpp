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
void Mage::Maths::Vector2::Save(std::ofstream & stream) const
{
	stream << x << "\n";
	stream << y << "\n";
}
void Mage::Maths::Vector2::Load(std::ifstream & stream)
{
	stream >> x; stream >> y;
}
Vector3::Vector3() : x(0), y(0), z(0)
{}
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)
{}
float Vector3::Length() const
{
	return sqrt(x*x + y * y + z * z);
}
Vector3 Vector3::Normalized() const
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
float Vector3::Dot(const Vector3 & other) const
{
	return x * other.x + y * other.y + z * other.z;
}
float Vector3::AngleBetween(const Vector3 & other) const
{
	Vector3 n0 = this->Normalized();
	Vector3 n1 = other.Normalized();
	return acos(n0.Dot(n1));
}
Vector3 Vector3::Cross(const Vector3 &other) const
{
	return Vector3(
		y*other.z - z * other.y,
		z*other.x - x * other.z,
		x*other.y - y * other.x);
}
Vector3 Mage::Maths::Vector3::Reflect(const Vector3 & normal) const
{
	Vector3 reflection = *this - normal * (2 * this->Dot(normal));
	return Vector3();
}
void Mage::Maths::Vector3::Save(std::ofstream & stream) const
{
	stream << x << "\n";
	stream << y << "\n";
	stream << z << "\n";
}
void Mage::Maths::Vector3::Load(std::ifstream & stream)
{
	stream >> x; stream >> y; stream >> z;
}
Vector4::Vector4() : x(0), y(0), z(0), w(0)
{}
Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
{}

Vector3i::Vector3i() : x(0), y(0) , z(0)
{
}

Vector3i::Vector3i(int x, int y, int z) : x(x), y(y), z(z)
{

}