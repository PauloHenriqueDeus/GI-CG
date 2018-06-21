#pragma once
#include "Object.h"
#include "Ray.h"
class Plano : public Object
{
private:

public:
	Plano(Vector3 position, Vector3 rotation, Material* _material) :Object(position, rotation, _material) {
		

	}
	~Plano();

	bool RayCast(Ray* ray, float& t0, float& t1) {



		return true;
	}

	Vector3 Normal(Vector3 point) {
		return transform.GetForward();
	}
};

