#pragma once
#include "Transform.h"
#include "Ray.h"
#include "Material.h"
#include <vector>

class Object
{
protected:
	Transform transform;
	Material* mat;


	Object(Vector3 position, Vector3 rotation, Material* _mat);
	~Object();

public:
	static std::vector<Object*>* GetObjects();

	static int GetObjectsLenght();

	virtual bool RayCast(Ray* ray, Vector3& hit) = 0;

	virtual Vector3 Normal(Vector3 point) = 0;

	Material* GetMaterial() {
		return mat;
	}

	Transform* GetTransform() {
		return &transform;
	}
};

