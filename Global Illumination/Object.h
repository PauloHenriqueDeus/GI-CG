#pragma once
#include "Transform.h"
#include "Ray.h"
#include "Material.h"
#include <vector>

class Object
{
private:
	static std::vector<Object*>objects;

protected:
	Transform transform;
	Material* mat;


	Object(Vector3 position, Material* _mat) {
		transform.Move(position);
		objects.push_back(this);
		mat = _mat;
	}
	~Object();

public:
	static Object** GetObjects() {
		return objects.data();
	}
	static int GetObjectsLenght() {
		return objects.size();
	}

	virtual bool RayCast(Ray* ray, Vector3& hit) = 0;

	virtual Vector3 Normal(Vector3 point) = 0;

	Material* GetMaterial() {
		return mat;
	}

	Transform* GetTransform() {
		return &transform;
	}
};

