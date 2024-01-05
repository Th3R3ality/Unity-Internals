#include "LapisTypes.h"
#include "GlobalDefines.h"

namespace Lapis
{
	////////////////////// Color

	Color::operator DXGI_RGBA()
	{
		return this->rgba;
	}

	Color Color::hex2rgba(const char* hex)
	{
		Color res;

		size_t len = strlen(hex);

		if (len < 8)
			res.a = 1;

		size_t at = 0;

		if (hex[0] == '#')
			at = 1;

		constexpr auto char2hex = [](char c) -> int {
			int res = 0;

			if (c >= '0' && c <= '9')
				return (res = c - '0');

			if (c >= 'a' && c <= 'f')
				c = c - ('a' - 'A');

			res = (c - 'A') + 10;

			return res;
			};

		constexpr auto hex2f = [](const char* hex) -> float {
			float out = 0;

			out = (out * 0x10) + char2hex(hex[0]);
			out = (out * 0x10) + char2hex(hex[1]);

			return out / static_cast<float>(0xff);
			};

		res.r = hex2f(hex + at); at += 2;
		res.g = hex2f(hex + at); at += 2;
		res.b = hex2f(hex + at); at += 2;
		if (at < len)
			res.a = hex2f(hex + at);

		return res;
	}

	////////////////////// Vec2

	Vec2 Lapis::Vec2::operator-() const
	{
		Vec2 res;

		res.x = -this->x;
		res.y = -this->y;

		return res;
	}
	Vec2 Vec2::operator+(const Vec2& other) const
	{
		Vec2 res;

		res.x = this->x + other.x;
		res.y = this->y + other.y;

		return res;
	}
	Vec2 Vec2::operator-(const Vec2& other) const
	{
		Vec2 res;

		res.x = this->x - other.x;
		res.y = this->y - other.y;

		return res;
	}
	Vec2 Vec2::operator*(const float& mul) const
	{
		Vec2 res;

		res.x = x * mul;
		res.y = y * mul;

		return res;
	}

	////////////////////// Vec3

	Vec3 const Vec3::right(1, 0, 0);
#if USE_Z_UP == 0
	Vec3 const Vec3::up(0, 1, 0);
	Vec3 const Vec3::forward(0, 0, 1);
#elif USE_Z_UP == 1
	Vec3 const Vec3::up(0, 0, 1);
	Vec3 const Vec3::forward(0, 1, 0);
#endif

	Vec3 Lapis::Vec3::operator-() const
	{
		Vec3 res;

		res.x = -this->x;
		res.y = -this->y;
		res.z = -this->z;

		return res;
	}

	Vec3 Vec3::operator+(const Vec3& other) const
	{
		Vec3 res;

		res.x = x + other.x;
		res.y = y + other.y;
		res.z = z + other.z;

		return res;
	}

	Vec3 Vec3::operator*(const float& scalar) const
	{
		Vec3 res;

		res.x = x * scalar;
		res.y = y * scalar;
		res.z = z * scalar;

		return res;
	}

	Vec3& Vec3::operator+=(const Vec3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}

	Vec3& Vec3::operator-=(const Vec3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}

	Vec3& Vec3::operator=(const UnityEngine::Vector3& other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;

		return *this;
	}

	////////////////////// Vec4

	Vec4 Lapis::Vec4::operator+(const Vec4& other) const
	{
		Vec4 res;

		res.x = this->x + other.x;
		res.y = this->y + other.y;
		res.z = this->z + other.z;
		res.w = this->w + other.w;

		return res;
	}

	Vec4& Vec4::operator=(const RECT& other)
	{

		x = static_cast<float>(other.left);
		y = static_cast<float>(other.top);
		z = static_cast<float>(other.right);
		w = static_cast<float>(other.bottom);

		return *this;
	}

	Vec4& Vec4::operator=(const DirectX::XMVECTOR& other)
	{
		this->x = other.m128_f32[0];
		this->y = other.m128_f32[1];
		this->z = other.m128_f32[2];
		this->w = other.m128_f32[3];

		return *this;
	}

	Vec4::operator DirectX::XMVECTOR()
	{
		DirectX::XMVECTOR res;

		res.m128_f32[0] = this->x;
		res.m128_f32[1] = this->y;
		res.m128_f32[2] = this->z;
		res.m128_f32[3] = this->w;

		return res;
	}

	////////////////////// mat4x4

	mat4x4& mat4x4::operator=(const DirectX::XMMATRIX& other)
	{
		this->r[0] = other.r[0];
		this->r[1] = other.r[1];
		this->r[2] = other.r[2];
		this->r[3] = other.r[3];

		return *this;
	}

	mat4x4::operator DirectX::XMMATRIX()
	{
		DirectX::XMMATRIX res;

		res.r[0] = this->r[0];
		res.r[1] = this->r[1];
		res.r[2] = this->r[2];
		res.r[3] = this->r[3];

		return res;
	}

	mat4x4::operator UnityEngine::Matrix4x4()
	{
		UnityEngine::Matrix4x4 res{};

		res.m00 = _00; res.m10 = _10; res.m20 = _20; res.m30 = _30;
		res.m01 = _01; res.m11 = _11; res.m21 = _21; res.m31 = _31;
		res.m02 = _02; res.m12 = _12; res.m22 = _22; res.m32 = _32;
		res.m03 = _03; res.m13 = _13; res.m23 = _23; res.m33 = _33;

		return res;
	}

	mat4x4 const mat4x4::Identity( { 1, 0, 0, 0 }, { 0,1,0,0 }, { 0,0,1,0 }, { 0,0,0,1 } );
}