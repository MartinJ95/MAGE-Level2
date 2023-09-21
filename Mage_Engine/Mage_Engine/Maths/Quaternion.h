#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include "Vector.h"

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
			Quaternion();
			Quaternion(float x, float y, float z, float w);
			Quaternion(const Maths::Vector3 &vector, float w);
			Quaternion(float theta, const Vector3& axis);
			void Save(std::ofstream& stream) const;
			void Load(std::ifstream& stream);
			Mage::Maths::Vector3 v;
			float w;
			const Quaternion operator *(const Quaternion& q);
			const Vector3 operator *(const Vector3& v);
			void operator =(const Quaternion& other);
		};
		Quaternion AngleAxis(const float &theta, const Vector3 &axis);
		Quaternion EulerToQuat(const Vector3 &e);
		Mage::Maths::Vector3 QuatToEuler(const Quaternion &q);
		Matrix4x4 QuatToMatrix(const Quaternion &q);
		Quaternion MatToQuat(const Matrix4x4 &m);
		Quaternion LookTowards(const Vector3 &dir, const Vector3 &up);
	}
}