#include "Matrix.h"
#include "Vector.h"
using namespace Mage::Maths;

Matrix4x4::Matrix4x4() : m0{ 0 }, m1{ 0 }, m2{0}, m3{0}
{}
Matrix4x4::Matrix4x4(const Vector3 &scale) : m0{ 1, 0, 0, 0 }, m1{ 0, 1, 0, 0 }, m2{ 0, 0, 1, 0 }, m3{0, 0, 0, 1}
{}
Matrix4x4::Matrix4x4(const Vector3 &scale, const Vector3 &position) : m0{ 1, 0, 0, position.x }, m1{ 0, 1, 0, position.y }, m2{ 0, 0, 1, position.z }, m3{ 0, 0, 0, 1 }
{}
Matrix4x4::Matrix4x4(const Vector3 &scale, const Vector3 &position, const Vector3 &euler) : m0{ 1, 0, 0, 0 }, m1{ 0, 1, 0, 0 }, m2{ 0, 0, 1, 0 }, m3{ 0, 0, 0, 1 }
{
	*this *= XRotateMatrix(euler.x); *this *= YRotateMatrix(euler.y); *this *= ZRotateMatrix(euler.z); Translate(position);
}
Matrix4x4::Matrix4x4(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33
) : m0{ m00, m01, m02, m03 }, m1{ m10, m11, m12, m13 }, m2{ m20, m21, m22, m23 }, m3{m30, m31, m32, m33}
{}
void Matrix4x4::Translate(const Vector3 &position)
{
	*this += Matrix4x4(
		0, 0, 0, position.x,
		0, 0, 0, position.y,
		0, 0, 0, position.z,
		0, 0, 0, 0
	);
}
void Matrix4x4::Scale(const Vector3 &scale)
{
	*this *= Matrix4x4(
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1
	);
}
void Matrix4x4::Scale(const float &scalar)
{
	*this *= Matrix4x4(
		scalar, 0, 0, 0,
		0, scalar, 0, 0,
		0, 0, scalar, 0,
		0, 0, 0, 1
	);
}
void Matrix4x4::Rotate(const Vector3 &euler)
{
	*this *= YRotateMatrix(euler.y);
	*this *= ZRotateMatrix(euler.z);
	*this *= XRotateMatrix(euler.x);
}
Matrix4x4 Mage::Maths::IdentityMatrix()
{
	return Matrix4x4(
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1);
}
Matrix4x4 Mage::Maths::XRotateMatrix(const float &theta)
{
	return Matrix4x4(
		1, 0, 0, 0,
		0, cos(theta), -sin(theta), 0,
		0, sin(theta), cos(theta), 0,
		0, 0, 0, 1
	);
}
Matrix4x4 Mage::Maths::YRotateMatrix(const float &theta)
{
	return Matrix4x4(
		cos(theta), 0, sin(theta), 0,
		0, 1, 0, 0,
		-sin(theta), 0, cos(theta), 0,
		0, 0, 0, 1
	);
}
Matrix4x4 Mage::Maths::ZRotateMatrix(const float &theta)
{
	return Matrix4x4(
		cos(theta), -sin(theta), 0, 0,
		sin(theta), cos(theta), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}