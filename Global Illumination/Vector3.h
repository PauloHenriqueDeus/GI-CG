#pragma once
#include <cmath>
class Vector3
{
public:
	float x, y, z;

	Vector3() {
		x = 0;
		y = 0;
		z = 0;
	}
	Vector3(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	bool AddValue(float v) {
		if (x == 0) {
			x = v;

			return true;
		}
		else if (y == 0) {
			y = v;

			return true;
		}
		else if (z == 0) {
			z = v;

			return true;
		}

		return false;
	}

	Vector3& operator+= (const Vector3& v) {
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
		return *this;
	}

	Vector3 operator* (const float f)
	{
		Vector3 t = *this;
		t.x *= f;
		t.y *= f;
		t.z *= f;
		return t;
	}

	Vector3 operator* (const Vector3& f)
	{
		Vector3 t = *this;
		t.x *= f.x;
		t.y *= f.y;
		t.z *= f.z;
		return t;
	}

	Vector3 operator/ (const float f)
	{
		Vector3 t = *this;
		t.x /= f;
		t.y /= f;
		t.z /= f;
		return t;
	}

	Vector3 operator/ (const Vector3 v)
	{
		Vector3 t = *this;
		t.x /= v.x;
		t.y /= v.y;
		t.z /= v.z;
		return t;
	}

	Vector3 operator+ (const Vector3& v)
	{
		Vector3 t = *this;
		t.x += v.x;
		t.y += v.y;
		t.z += v.z;
		return t;	
	}

	Vector3 operator- (const Vector3& v)
	{
		Vector3 t = *this;
		t.x -= v.x;
		t.y -= v.y;
		t.z -= v.z;
		return t;
	}

	Vector3 operator- ()
	{
		Vector3 t = *this;
		t.x = -t.x;
		t.y = -t.y;
		t.z = -t.z;
		return t;
	}

	static Vector3 Cross(Vector3 a, Vector3 b) {

		return Vector3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
	}

	static float Dot(Vector3 a, Vector3 b) {

		return (a.x * b.x + a.y * b.y + a.z * b.z)/(a.Magnitude()*b.Magnitude());
	}

	static float DotNoMag(Vector3 a, Vector3 b) {

		return (a.x * b.x + a.y * b.y + a.z * b.z);
	}

	float Magnitude() {
		return sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
	}

	Vector3 Normalized() {
		return (*this) / Magnitude();
	}

	Vector3 Reflection(Vector3* normal) {
		Vector3 rin = -*this;

		float cos = normal->x * rin.x + normal->y * rin.y + normal->z * rin.z;
		Vector3 rout = Vector3(2 * normal->x * cos - rin.x, 2 * normal->y * cos - rin.y, 2 * normal->z * cos - rin.z);

		return rout;
	}

	Vector3 Refraction(Vector3* normal, float c1, float c2) {

		/*
		sf::Vector2f rin = (ray[0].position - ray[1].position);
		sf::Vector2f n = normal;

		float rayMagnitude = Magnitude(rin);

		rin = Normalize(rin);
		n = Normalize(n);

		float cos1 = n.x * rin.x + n.y * rin.y;
		float cos2 = sqrtf(1 - powf(c1 / c2, 2)*(1 - powf(cos1, 2)));

		sf::Vector2f rout = sf::Vector2f(
			(c1 / c2)*(-rin.x) + (cos1 >= 0 ? (c1 / c2 * cos1 - cos2) : (c1 / c2 * cos1 + cos2)) *n.x,
			(c1 / c2)*(-rin.y) + (cos1 >= 0 ? (c1 / c2 * cos1 - cos2) : (c1 / c2 * cos1 + cos2)) *n.y);

		return rout*rayMagnitude;
		*/

		Vector3 rin = -*this;
		
		float cos = normal->x * rin.x + normal->y * rin.y + normal->z * rin.z;
		float cos1 = sqrtf(1 - powf(c1 / c2, 2)*(1 - powf(cos, 2)));

		Vector3 rout = Vector3(
			(c1 / c2)*(-rin.x) + (cos >= 0 ? (c1 / c2 * cos - cos1) : (c1 / c2 * cos + cos1)) *normal->x,
			(c1 / c2)*(-rin.y) + (cos >= 0 ? (c1 / c2 * cos - cos1) : (c1 / c2 * cos + cos1)) *normal->y,
			(c1 / c2)*(-rin.z) + (cos >= 0 ? (c1 / c2 * cos - cos1) : (c1 / c2 * cos + cos1)) *normal->z);

		return rout;
	}

	~Vector3();
};

