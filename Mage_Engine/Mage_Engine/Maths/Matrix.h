#pragma once
#include <math.h>
namespace Mage
{
	namespace Maths
	{
		class Vector3;
	}
}
namespace Mage
{
	namespace Maths
	{
		class Matrix4x4
		{
		public:
			Matrix4x4();
			Matrix4x4(const Vector3 &scale);
			Matrix4x4(const Vector3 &scale, const Vector3 &position);
			Matrix4x4(const Vector3 &scale, const Vector3 &position, const Vector3 &euler);
			Matrix4x4(float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23,
				float m30, float m31, float m32, float m33);
			float m0[4], m1[4], m2[4], m3[4];
			void Translate(const Vector3 &position);
			void Scale(const Vector3 &scale);
			void Scale(const float &scalar);
			void Rotate(const Vector3 &euler);
			Matrix4x4 operator+(const Matrix4x4 &other)
			{
				return Matrix4x4(
					m0[0] + other.m0[0], m0[1] + other.m0[1], m0[2] + other.m0[2], m0[3] + other.m0[3],
					m1[0] + other.m1[0], m1[1] + other.m1[1], m1[2] + other.m1[2], m1[3] + other.m1[3],
					m2[0] + other.m2[0], m2[1] + other.m2[1], m2[2] + other.m2[2], m2[3] + other.m2[3],
					m3[0] + other.m3[0], m3[1] + other.m3[1], m3[2] + other.m3[2], m3[3] + other.m3[3]
				);
			}
			void operator+=(const Matrix4x4 &other)
			{
				m0[0] += other.m0[0]; m0[1] += other.m0[1]; m0[2] += other.m0[2]; m0[3] += other.m0[3];
				m1[0] += other.m1[0]; m1[1] += other.m1[1]; m1[2] += other.m1[2]; m1[3] += other.m1[3];
				m2[0] += other.m2[0]; m2[1] += other.m2[1]; m2[2] += other.m2[2]; m2[3] += other.m2[3];
				m3[0] += other.m3[0]; m3[1] += other.m3[1]; m3[2] += other.m3[2]; m3[3] += other.m3[3];
			}
			Matrix4x4 operator*(const float &scalar)
			{
				return Matrix4x4(
					m0[0] * scalar, m0[1] * scalar, m0[2] * scalar, m0[3] * scalar,
					m1[0] * scalar, m1[1] * scalar, m1[2] * scalar, m1[3] * scalar,
					m2[0] * scalar, m2[1] * scalar, m2[2] * scalar, m2[3] * scalar,
					m3[0] * scalar, m3[1] * scalar, m3[2] * scalar, m3[3] * scalar
				);
			}
			void operator*=(const float &scalar)
			{
				m0[0] *= scalar; m0[1] *= scalar; m0[2] *= scalar; m0[3] *= scalar;
				m1[0] *= scalar; m1[1] *= scalar; m1[2] *= scalar; m1[3] *= scalar;
				m2[0] *= scalar; m2[1] *= scalar; m2[2] *= scalar; m2[3] *= scalar;
				m3[0] *= scalar; m3[1] *= scalar; m3[2] *= scalar; m3[3] *= scalar;
			}
			Matrix4x4 operator*(const Matrix4x4 &other)
			{
				return Matrix4x4(
					MatrixMultRowColumn(m0, 0, other), MatrixMultRowColumn(m0, 1, other), MatrixMultRowColumn(m0, 2, other), MatrixMultRowColumn(m0, 3, other),
					MatrixMultRowColumn(m1, 0, other), MatrixMultRowColumn(m1, 1, other), MatrixMultRowColumn(m1, 2, other), MatrixMultRowColumn(m1, 3, other),
					MatrixMultRowColumn(m2, 0, other), MatrixMultRowColumn(m2, 1, other), MatrixMultRowColumn(m2, 2, other), MatrixMultRowColumn(m2, 3, other),
					MatrixMultRowColumn(m3, 0, other), MatrixMultRowColumn(m3, 1, other), MatrixMultRowColumn(m3, 2, other), MatrixMultRowColumn(m3, 3, other)
					);
			}
			void operator*=(const Matrix4x4 &other)
			{
				*this = *this * other;
			}
		private:
			// helper function for multiplying matrices clamping row and column
			float MatrixMultRowColumn(const float (&row)[4], const int &column, const Matrix4x4 &other)
			{
				return row[0] * other.m0[column] + row[1] * other.m1[column] + row[2] * other.m2[column] + row[3] * other.m3[column];
			}
		};
		Matrix4x4 IdentityMatrix();
		Matrix4x4 XRotateMatrix(const float &theta);
		Matrix4x4 YRotateMatrix(const float &theta);
		Matrix4x4 ZRotateMatrix(const float &theta);
	}
}