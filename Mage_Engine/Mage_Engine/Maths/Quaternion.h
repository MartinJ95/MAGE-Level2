#pragma once
#include <math.h>
#define _USE_MATH_DEFINES

namespace Mage
{
	namespace Maths
	{
		class Vector3;
		class Matrix4x4;
	}
}

namespace Mage
{
	namespace Maths
	{
		class Quaternion
		{
		public:
			Quaternion(float x, float y, float z, float w);
			float x, y, z, w;
		};
		Quaternion AngleAxis(const float &theta, const Vector3 &axis);
		Quaternion EulerToQuat(const Vector3 &e);
		Vector3 QuatToEuler(const Quaternion &q);
		Matrix4x4 QuatToMatrix(const Quaternion &q);
		Quaternion MatToQuat(const Matrix4x4 &m);
		Quaternion LookTowards(const Vector3 &dir, const Vector3 &up);
	}
}