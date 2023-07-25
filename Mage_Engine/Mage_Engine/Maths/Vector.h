#pragma once
#include <math.h>
#include <iostream>
#include <fstream>
#include "Matrix.h"

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
			void Save(std::ofstream &stream) const;
			void Load(std::ifstream &stream);
			float x, y;
			Vector2 operator*(const float &scalar) const { return Vector2(x*scalar, y*scalar); }
			Vector2 operator+(const float &scalar) const { return Vector2(x + scalar, y + scalar); }
			Vector2 operator-(const float &scalar) const { return Vector2(x - scalar, y - scalar); }
			void operator*=(const float &scalar) { x = x * scalar; y = y * scalar; }
			void operator+=(const float &scalar) { x = x + scalar; y = y + scalar; }
			void operator-=(const float &scalar) { x = x - scalar; y = y - scalar; }
			bool operator>(const float &f) const { if (x > f && y > f) { return true; }return false; }
			bool operator<(const float &f) const { if (x < f && y < f) { return true; }return false; }
			Vector2 operator*(const Vector2 &other) const { return Vector2(x * other.x, y * other.y); }
			Vector2 operator+(const Vector2 &other) const { return Vector2(x + other.x, y + other.y); }
			Vector2 operator-(const Vector2 &other) const { return Vector2(x - other.x, y - other.y); }
			void operator*=(const Vector2 &other) { x = x * other.x; y = y * other.y; }
			void operator+=(const Vector2 &other) { x = x + other.x; y = y + other.y; }
			void operator-=(const Vector2 &other) { x = x - other.x; y = y - other.y; }
			bool operator==(const Vector2 &other) const { if (x == other.x && y == other.y) { return true; }return false; }
			bool operator!=(const Vector2 &other) const { if (x != other.x || y != other.y) { return true; }return false; }
			bool operator>(const Vector2 &other) const { if (x > other.x && y > other.y) { return true; }return false; }
			bool operator<(const Vector2 &other) const { if (x < other.x || y < other.y) { return true; }return false; }
		};
		class Vector3
		{
		public:
			Vector3();
			Vector3(float x, float y, float z);
			float Length() const;
			Vector3 Normalized() const;
			void NormalizeInPlace();
			float Dot(const Vector3 &other) const;
			float AngleBetween(const Vector3 &other) const;
			Vector3 Cross(const Vector3 &other) const;
			Vector3 Reflect(const Vector3 &normal) const;
			void Save(std::ofstream &stream) const;
			void Load(std::ifstream &stream);
			float x, y, z;
			Vector3 operator*(const float &scalar) const { return Vector3(x*scalar, y*scalar, z*scalar); }
			Vector3 operator+(const float &scalar) const { return Vector3(x + scalar, y + scalar, z + scalar); }
			Vector3 operator-(const float &scalar) const { return Vector3(x - scalar, y - scalar, z - scalar); }
			Vector3 operator/(const float &scalar) const { return Vector3(x / scalar, y / scalar, z / scalar); }
			void operator*=(const float &scalar) { x = x * scalar; y = y * scalar; z = z * scalar; }
			void operator+=(const float &scalar) { x = x + scalar; y = y + scalar; z = z + scalar; }
			void operator-=(const float &scalar) { x = x - scalar; y = y - scalar; z = z - scalar; }
			bool operator>(const float &f) const { if (x > f && y > f && z > f) {return true;}return false;}
			bool operator<(const float &f) const { if (x < f && y < f && z < f) {return true;}return false;}
			Vector3 operator*(const Vector3 &other) const { return Vector3(x * other.x, y*other.y, z*other.z); }
			Vector3 operator+(const Vector3 &other) const { return Vector3(x + other.x, y+other.y, z+other.z); }
			Vector3 operator-(const Vector3 &other) const { return Vector3(x - other.x, y-other.y, z-other.z); }
			Vector3 operator-() { return Vector3(-x, -y, -z); }
			void operator=(const Vector3 &other) { x = other.x; y = other.y; z = other.z; }
			void operator*=(const Vector3 &other) { x = x * other.x; y = y * other.y; z = z * other.z; }
			void operator+=(const Vector3 &other) { x = x + other.x; y = y + other.y; z = z + other.z; }
			void operator-=(const Vector3 &other) { x = x - other.x; y = y - other.y; z = z - other.z; }
			bool operator==(const Vector3 &other) const { if (x == other.x &&y == other.y&&z == other.z) { return true; }return false; }
			bool operator>(const Vector3 &other) const { if (x > other.x && y > other.y && z > other.z) { return true; }return false; }
			bool operator<(const Vector3 &other) const { if (x < other.x || y < other.y && z < other.z) { return true; }return false; }
		};
		class Vector3i
		{
		public:
			Vector3i();
			Vector3i(int x, int y, int z);
			int x, y, z;
			Vector3i operator*(const int &scalar) const { return Vector3i(x*scalar, y*scalar, z*scalar); }
			Vector3i operator+(const int &scalar) const { return Vector3i(x + scalar, y + scalar, z + scalar); }
			Vector3i operator-(const int &scalar) const { return Vector3i(x - scalar, y - scalar, z - scalar); }
			void operator*=(const int &scalar) { x = x * scalar; y = y * scalar; z = z * scalar; }
			void operator+=(const int &scalar) { x = x + scalar; y = y + scalar; z = z + scalar; }
			void operator-=(const int &scalar) { x = x - scalar; y = y - scalar; z = z - scalar; }
			Vector3i operator*(const Vector3 &other) const { return Vector3i(x * other.x, y*other.y, z*other.z); }
			Vector3i operator+(const Vector3 &other) const { return Vector3i(x + other.x, y + other.y, z + other.z); }
			Vector3i operator-(const Vector3 &other) const { return Vector3i(x - other.x, y - other.y, z - other.z); }
			void operator*=(const Vector3 &other) { x = x * other.x; y = y * other.y; z = z * other.z; }
			void operator+=(const Vector3 &other) { x = x + other.x; y = y + other.y; z = z + other.z; }
			void operator-=(const Vector3 &other) { x = x - other.x; y = y - other.y; z = z - other.z; }
			bool operator==(const Vector3 &other) const { if (x == other.x &&y == other.y&&z == other.z) { return true; }return false; }
		};
		class Vector4
		{
		public:
			Vector4();
			Vector4(float x, float y, float z, float w);
			Vector4 operator*(const Mage::Maths::Matrix4x4 &mat) {
				return Mage::Maths::Vector4(
					x * mat.m0[0] + x * mat.m0[1] + x * mat.m0[2] + x * mat.m0[3],
					y * mat.m1[0] + y * mat.m1[1] + y * mat.m1[2] + y * mat.m1[3],
					z * mat.m2[0] + z * mat.m2[1] + z * mat.m2[2] + z * mat.m2[3],
					w * mat.m3[0] + w * mat.m3[1] + w * mat.m3[2] + w * mat.m3[3]);
			}
			void operator=(const Mage::Maths::Vector4 &other) { x = other.x; y = other.y; z = other.z; w = other.w; }
			bool operator==(const Mage::Maths::Vector4 &other) { if (x == other.x && y == other.y, z == other.z && w == other.w) { return true; } else { return false; } }
			float x, y, z, w;
		};
	}
}