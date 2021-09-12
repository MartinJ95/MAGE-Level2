#include "Quaternion.h"
#include "Vector.h"
#include "Matrix.h"

using namespace Mage::Maths;

Quaternion::Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
{}

Quaternion Mage::Maths::AngleAxis(const float & theta, const Vector3 & axis)
{
	return Quaternion(
		axis.x * sin(theta / 2),
		axis.y * sin(theta / 2),
		axis.z * sin(theta / 2),
		cos(theta / 2));
}

Quaternion Mage::Maths::EulerToQuart(const Mage::Maths::Vector3 & e)
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

Vector3 Mage::Maths::QuartToEuler(const Quaternion &q)
{
	float heading;
	float attitude;
	float bank;
	float testNum = q.x * q.y + q.z * q.w;
	if (testNum > 0.499)
	{
		heading = 2 * atan2(q.x, q.w);
		attitude = M_PI / 2;
		bank = 0;
	}
	else if (testNum < -0.499)
	{
		heading = -2 * atan2(q.x, q.w);
		attitude = M_PI / 2;
		bank = 0;
	}
	else
	{
		heading = atan2(2 * q.y * q.w - 2 * q.x * q.z, 1 - 2 * (q.y * q.y) - 2 * (q.z * q.z));
		attitude = asin(2 * q.x * q.y + 2 * q.z * q.w);
		bank = atan2(2 * q.x * q.w - 2 * q.y * q.z, 1 - 2 * (q.x * q.x) - 2 * (q.z * q.z));
	}
	return Vector3(bank, heading, attitude);
}

Matrix4x4 Mage::Maths::QuartToMatrix(const Quaternion &q)
{
	return Matrix4x4(
		1 - 2 * (q.y * q.y) - 2 * (q.z * q.z), 2 * q.x * q.y - 2 * q.z * q.w, 2 * q.x * q.z + 2 * q.y * q.w, 0,
		2 * q.x * q.y + 2 * q.z * q.w, 1 - 2 * (q.x * q.x) - 2 * (q.z * q.z), 2 * q.y * q.z - 2 * q.x * q.w, 0,
		2 * q.x * q.z - 2 * q.y * q.w, 2 * q.y * q.z + 2 * q.x * q.w, 1 - 2 * (q.x * q.x) - 2 * (q.y * q.y), 0,
		0, 0, 0, 1);
}