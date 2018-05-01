#pragma once
#include "Vector3.h"
class Ray
{
private:
	Vector3* origin;
	Vector3* direction;
public:
	Ray(Vector3 _origin, Vector3 _direction) {
		origin = new Vector3(_origin);
		direction = new Vector3(_direction);
	}
	/*Ray(Vector3 start, Vector3 end) {
		origin = start;
		direction = end - start;
	}*/
	~Ray() {
		delete origin;
		delete direction;
	}

	Vector3* GetOrigin() {
		return origin;
	}
	Vector3* GetDirection() {
		return direction;
	}
	Vector3 GetEnd() {
		return *origin + *direction;
	}
	float GetSize() {
		return direction->Magnitude();
	}

	Ray Reflection(Vector3 normal) {
		Vector3 rin = -direction->Normalized();
		normal = normal.Normalized();

		float cos = normal.x * rin.x + normal.y * rin.y;
		Vector3 rout = Vector3(2 * normal.x * cos - rin.x, 2 * normal.y * cos - rin.y, 2 * normal.z * cos - rin.z)*direction->Magnitude();

		return Ray(*origin + *direction, rout);
	}

	Ray Refracton(Vector3 normal, float c1, float c2) {
		Vector3 rin = -direction->Normalized();
		normal = normal.Normalized();

		float cos1 = normal.x * rin.x + normal.y * rin.y;
		float cos2 = sqrtf(1 - powf(c1 / c2, 2)*(1 - powf(cos1, 2)));

		Vector3 rout = Vector3(
			(c1 / c2)*(-rin.x) + (cos1 >= 0 ? (c1 / c2 * cos1 - cos2) : (c1 / c2 * cos1 + cos2)) *normal.x,
			(c1 / c2)*(-rin.y) + (cos1 >= 0 ? (c1 / c2 * cos1 - cos2) : (c1 / c2 * cos1 + cos2)) *normal.y,
			(c1 / c2)*(-rin.z) + (cos1 >= 0 ? (c1 / c2 * cos1 - cos2) : (c1 / c2 * cos1 + cos2)) *normal.z) * direction->Magnitude();

		return Ray(*origin + *direction, rout);
	}

	Ray operator* (float f) {
		Ray r = *this;
		*r.direction = r.direction->Normalized()*f;
		return r;
	}
};

