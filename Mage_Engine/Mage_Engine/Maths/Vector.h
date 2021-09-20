#pragma once
#include <math.h>

namespace Mage
{
	namespace Maths
	{
		class Vector2
		{
		public:
			Vector2();
			Vector2(float x, float y);
			float Length();
			Vector2 Normalized();
			void NormalizeInPlace();
			float Dot(const Vector2 &other);
			float x, y;
			Vector2 operator*(const float &scalar) const { return Vector2(x*scalar, y*scalar); }
			Vector2 operator+(const float &scalar) const { return Vector2(x + scalar, y + scalar); }
			Vector2 operator-(const float &scalar) const { return Vector2(x - scalar, y - scalar); }
			void operator*=(const float &scalar) { x = x * scalar; y = y * scalar; }
			void operator+=(const float &scalar) { x = x + scalar; y = y + scalar; }
			void operator-=(const float &scalar) { x = x - scalar; y = y - scalar; }
			Vector2 operator*(const Vector2 &other) const { return Vector2(x * other.x, y * other.y); }
			Vector2 operator+(const Vector2 &other) const { return Vector2(x + other.x, y + other.y); }
			Vector2 operator-(const Vector2 &other) const { return Vector2(x - other.x, y - other.y); }
			void operator*=(const Vector2 &other) { x = x * other.x; y = y * other.y; }
			void operator+=(const Vector2 &other) { x = x + other.x; y = y + other.y; }
			void operator-=(const Vector2 &other) { x = x - other.x; y = y - other.y; }
			bool operator==(const Vector2 &other) const { if (x == other.x && y == other.y) { return true; }return false; }
		};
		class Vector3
		{
		public:
			Vector3();
			Vector3(float x, float y, float z);
			float Length();
			Vector3 Normalized();
			void NormalizeInPlace();
			float Dot(const Vector3 &other);
			Vector3 Cross(const Vector3 &other) const;
			float x, y, z;
			Vector3 operator*(const float &scalar) const { return Vector3(x*scalar, y*scalar, z*scalar); }
			Vector3 operator+(const float &scalar) const { return Vector3(x + scalar, y + scalar, z + scalar); }
			Vector3 operator-(const float &scalar) const { return Vector3(x - scalar, y - scalar, z - scalar); }
			void operator*=(const float &scalar) { x = x * scalar; y = y * scalar; z = z * scalar; }
			void operator+=(const float &scalar) { x = x + scalar; y = y + scalar; z = z + scalar; }
			void operator-=(const float &scalar) { x = x - scalar; y = y - scalar; z = z - scalar; }
			Vector3 operator*(const Vector3 &other) const { return Vector3(x * other.x, y*other.y, z*other.z); }
			Vector3 operator+(const Vector3 &other) const { return Vector3(x + other.x, y+other.y, z+other.z); }
			Vector3 operator-(const Vector3 &other) const { return Vector3(x - other.x, y-other.y, z-other.z); }
			void operator*=(const Vector3 &other) { x = x * other.x; y = y * other.y; z = z * other.z; }
			void operator+=(const Vector3 &other) { x = x + other.x; y = y + other.y; z = z + other.z; }
			void operator-=(const Vector3 &other) { x = x - other.x; y = y - other.y; z = z - other.z; }
			bool operator==(const Vector3 &other) const { if (x == other.x &&y == other.y&&z == other.z) { return true; }return false; }
		};
		class Vector4
		{
			Vector4();
			Vector4(float x, float y, float z, float w);
			float x, y, z, w;
		};
	}
}