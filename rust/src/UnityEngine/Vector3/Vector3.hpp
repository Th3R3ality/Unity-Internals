#pragma once
#include <ostream>
#include <string>

#include "Il2cppLib.h"

namespace UnityEngine
{
	struct Vector3
	{
		float x, y, z;

		Vector3() : x(0), y(0), z(0) {}
		Vector3(float x) : x(x), y(x), z(x) {}
		Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

		friend std::ostream& operator<<(std::ostream& os, const Vector3& v3)
		{
			os << v3.x << '/' << v3.y << '/' << v3.z;
			return os;
		}
		friend Vector3& operator*=(Vector3& vector, const int& i)
		{
			vector.x *= i; vector.y *= i; vector.z *= i;
			return vector;
		}
		friend Vector3& operator*=(Vector3& v, const float& f)
		{
			v.x *= f;
			v.y *= f;
			v.z *= f;
			return v;
		}
		Vector3 operator*(float f)
		{
			Vector3 res;

			res.x = x * f;
			res.y = y * f;
			res.z = z * f;

			return res;
		}
		Vector3& operator+(float f)
		{
			x += f;
			y += f;
			z += f;
			return *this;
		}
		bool operator==(Vector3 right);

		Vector3 operator+(Vector3 other)
		{
			Vector3 res;

			res.x = x + other.x;
			res.y = y + other.y;
			res.z = z + other.z;

			return res;
		}
		friend Vector3& operator+=(Vector3& v1, const Vector3& v2)
		{
			v1.x += v2.x;
			v1.y += v2.y;
			v1.z += v2.z;
			return v1;
		}
		friend Vector3& operator-=(Vector3& v1, const Vector3& v2)
		{
			v1.x -= v2.x;
			v1.y -= v2.y;
			v1.z -= v2.z;
			return v1;
		}
		friend Vector3& operator-(Vector3& v1, const Vector3& v2)
		{
			v1.x -= v2.x;
			v1.y -= v2.y;
			v1.z -= v2.z;
			return v1;
		}
		friend Vector3& operator-(Vector3 v1, Vector3 v2)
		{
			v1.x -= v2.x;
			v1.y -= v2.y;
			v1.z -= v2.z;
			return v1;
		}

		static Vector3 zero()
		{
			static auto fn = (Vector3(*)())Il2cppLib::virtual_method_from_signature("UnityEngine::Vector3.get_zero()");
			if (fn)
				return fn();
			return { 0 };
		}

		std::string toString()
		{
			return std::to_string(this->x) + "/" + std::to_string(this->y) + "/" + std::to_string(this->z);
		}

		static Vector3 Normalize(Vector3 v)
		{
			static auto Normalize_method = (Vector3(*)(Vector3))Il2cppLib::virtual_method_from_signature("UnityEngine::Vector3.Normalize(Vector3)");
			return Normalize_method(v);
		}

		static float Magnitude(Vector3 v)
		{
			static auto Magnitude_method = (float(*)(Vector3))Il2cppLib::virtual_method_from_signature("UnityEngine::Vector3.Magnitude(Vector3)");
			return Magnitude_method(v);
		}

		static float Distance(Vector3 a, Vector3 b)
		{
			static auto Distance_method = (float(*)(Vector3, Vector3))Il2cppLib::virtual_method_from_signature("UnityEngine::Vector3.Distance(Vector3, Vector3)");
			return Distance_method(a, b);
		}
		static float Distance2D(Vector3 a, Vector3 b)
		{
			Vector3 _a = a; _a.y = 0;
			Vector3 _b = a; _b.y = 0;
			return Distance(_a, _b);
		}
	};
}