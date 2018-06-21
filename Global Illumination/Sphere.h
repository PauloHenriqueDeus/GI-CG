#pragma once
#include "Object.h"
#include <iostream>

class Sphere : public Object
{
public:
	Sphere(Vector3 position, Vector3 rotation, float radius, Material* _material) :Object(position, rotation, _material) {
		transform.SetScale(Vector3(1, 1, 1) * radius);
	}
	~Sphere() {};

	bool RayCast(Ray* ray, Vector3& hit) {
		Vector3 o = *ray->GetOrigin();
		Vector3 c = *transform.GetPosition();
		float sRadius = transform.GetScale()->x;

		Vector3 d = ray->GetDirection()->Normalized();

		float a = (d.x*d.x + d.y*d.y + d.z*d.z);
		float b = 2 * (d.x*(o.x - c.x) + d.y*(o.y - c.y) + d.z*(o.z - c.z));
		float _c = powf(o.x - c.x, 2) + powf(o.y - c.y, 2) + powf(o.z - c.z, 2) - powf(sRadius, 2);

		float delta = (b*b) - (4.0f* a*_c);

		if (delta < 0) return false;

		float t0 = -b - sqrtf(delta) / 2.0f;
		float t1 = -b + sqrtf(delta) / 2.0f;

		float t = (t0 < t1) ? t0 : t1;

		if (t < 0) {
			return false;
		}

		hit = (*ray->GetOrigin()) + ray->GetDirection()->Normalized() * t / 2.0f - ((-ray->GetDirection()->Normalized())*((t0 - t1) / 2.0f) / 2.0f);

		return true;
	}

	Vector3 Normal(Vector3 point) {
		return ((point - *transform.GetPosition())) / transform.GetScale()->x;
	}
};

