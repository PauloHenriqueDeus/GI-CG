#pragma once
#include "Object.h"
#include "Ray.h"
class AACube : public Object
{
public:
	AACube(Vector3 position, Vector3 size, Material* _material) :Object(position, _material) {
		transform.SetScale(size);
	}
	~AACube();

	bool RayCast(Ray* ray, Vector3& hit) {

		float d = (-transform.GetForward().x*transform.GetPosition()->x) +
			(-transform.GetForward().y*transform.GetPosition()->y) +
			(-transform.GetForward().z*transform.GetPosition()->z);

		float t = -(((transform.GetForward().x*ray->GetOrigin()->x) + (transform.GetForward().y*ray->GetOrigin()->y) + (transform.GetForward().z*ray->GetOrigin()->z) + d) /
			((transform.GetForward().x*ray->GetDirection()->x) + (transform.GetForward().y*ray->GetDirection()->y) + (transform.GetForward().z*ray->GetDirection()->z)));

		
		if (t < 0) return false;

		hit = ((*ray->GetOrigin()) + *ray->GetDirection() * t);
		Vector3 v = hit-*transform.GetPosition();

		Vector3 r = transform.GetRight() * (Vector3::DotNoMag(v, transform.GetRight()));
		Vector3 u = transform.GetUp() * (Vector3::DotNoMag(v, transform.GetUp()));

		if ((r.Magnitude() <= transform.GetScale()->x / 2) && (u.Magnitude() <= transform.GetScale()->y / 2)) {
			return true;
		}

		return false;
	}

	Vector3 Normal(Vector3 point) {
		return transform.GetForward();
	}
};
