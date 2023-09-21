#include "Maths/Quaternion.h"
#include "Maths/Matrix.h"

using namespace Mage::Maths;

Mage::Maths::Quaternion::Quaternion() : v(0.f, 0.f, 0.f), w(1.f)
{
}

Mage::Maths::Quaternion::Quaternion(float x, float y, float z, float w) : v(x, y, z), w(w)
{}

Mage::Maths::Quaternion::Quaternion(const Mage::Maths::Vector3& vector, float w) : v(vector), w(w)
{
}

Mage::Maths::Quaternion::Quaternion(float theta, const Vector3 & axis)
{
	*this = AngleAxis(theta, axis);
}

void Mage::Maths::Quaternion::Save(std::ofstream& stream) const
{
	v.Save(stream);
	stream << w << "\n";
}

void Mage::Maths::Quaternion::Load(std::ifstream& stream)
{
	v.Load(stream);
	stream >> w;
}


const Quaternion Mage::Maths::Quaternion::operator*(const Quaternion& q)
{
	/*Quaternion p(
		v * q.w + q.v * w + v.Cross(q.v),
		w*q.w + v.Dot(q.v)
	);*/
	Quaternion p(
		Mage::Maths::Vector3(
			v.x * q.w + v.y * q.v.z - v.z * q.v.y + w * q.v.x,
			-v.x * q.v.z + v.y * q.w + v.z * q.v.x + w * q.v.y,
			v.x * q.v.y - v.y * q.v.x + v.z * q.w + w * q.v.z
		),
		-v.x * q.v.x - v.y * q.v.y - v.z * q.v.z + w * q.w
	);
	return p;
}

const Vector3 Mage::Maths::Quaternion::operator*(const Vector3& V)
{
	Quaternion p(V, 0);

	Vector3 vcV = v.Cross(V);
	return V + vcV * (2 * w) + v.Cross(vcV) * 2;
}

void Mage::Maths::Quaternion::operator=(const Quaternion& other)
{
	v = other.v;
	w = other.w;
}

Quaternion Mage::Maths::AngleAxis(const float & theta, const Vector3 & axis)
{
	float radians = theta / 360 * (float)M_PI * 2;

	return Quaternion(
		axis.x * sin(radians / 2),
		axis.y * sin(radians / 2),
		axis.z * sin(radians / 2),
		cos(radians / 2));
}

Quaternion Mage::Maths::EulerToQuat(const Mage::Maths::Vector3 & e)
{
	//y == heading
	//z == attitude
	//x == bank
	//cosine
	Vector3 c(cos(e.y) / 2, cos(e.z) / 2, cos(e.x) / 2);
	//sine
	Vector3 s(sin(e.y) / 2, sin(e.z) / 2, sin(e.x) / 2);
	return Mage::Maths::Quaternion(
		s.x * s.y * c.z + c.x * c.y * s.z,
		s.x * c.y * c.z + c.x * s.y * s.z,
		s.x * c.y * c.z - c.x * s.y * s.z,
		c.x * c.y * c.z - s.x * s.y * s.z
	);
}

Vector3 Mage::Maths::QuatToEuler(const Quaternion &q)
{
	float heading;
	float attitude;
	float bank;
	float testNum = q.v.x * q.v.y + q.v.z * q.w;
	if (testNum > 0.499)
	{
		heading = 2 * atan2(q.v.x, q.w);
		attitude = M_PI / 2;
		bank = 0;
	}
	else if (testNum < -0.499)
	{
		heading = -2 * atan2(q.v.x, q.w);
		attitude = M_PI / 2;
		bank = 0;
	}
	else
	{
		heading = atan2(2 * q.v.y * q.w - 2 * q.v.x * q.v.z, 1 - 2 * (q.v.y * q.v.y) - 2 * (q.v.z * q.v.z));
		attitude = asin(2 * q.v.x * q.v.y + 2 * q.v.z * q.w);
		bank = atan2(2 * q.v.x * q.w - 2 * q.v.y * q.v.z, 1 - 2 * (q.v.x * q.v.x) - 2 * (q.v.z * q.v.z));
	}
	return Vector3(bank, heading, attitude);
}

Matrix4x4 Mage::Maths::QuatToMatrix(const Quaternion &q)
{
	return Matrix4x4(
		1 - 2 * (q.v.y * q.v.y) - 2 * (q.v.z * q.v.z), 2 * q.v.x * q.v.y - 2 * q.v.z * q.w, 2 * q.v.x * q.v.z + 2 * q.v.y * q.w, 0,
		2 * q.v.x * q.v.y + 2 * q.v.z * q.w, 1 - 2 * (q.v.x * q.v.x) - 2 * (q.v.z * q.v.z), 2 * q.v.y * q.v.z - 2 * q.v.x * q.w, 0,
		2 * q.v.x * q.v.z - 2 * q.v.y * q.w, 2 * q.v.y * q.v.z + 2 * q.v.x * q.w, 1 - 2 * (q.v.x * q.v.x) - 2 * (q.v.y * q.v.y), 0,
		0, 0, 0, 1);
}

Quaternion Mage::Maths::MatToQuat(const Matrix4x4 & m)
{
	float W = sqrt((1 + m.m0[0] + m.m1[1] + m.m2[2]) / 2);
	float QW = 4 * W;
	return Quaternion(
		(m.m2[1] - m.m1[2]) * QW,
		(m.m0[2] - m.m2[0]) * QW,
		(m.m1[0] - m.m0[1]) * QW,
		W);
}

Quaternion Mage::Maths::LookTowards(const Vector3 & dir, const Vector3 & up)
{
	Vector3 S = dir.Cross(up);
	Vector3 U = dir.Cross(S);

	Matrix4x4 M(
		dir.x, dir.y, dir.z, 0,
		U.x, U.y, U.z, 0,
		S.x, S.y, S.z, 0,
		0, 0, 0, 1);
	return MatToQuat(M);
}
