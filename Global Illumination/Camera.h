#pragma once
#include "Transform.h"

class Camera
{
private:
	float fov;

	Transform transform;
public:
	Camera() {
		transform.Rotate(Vector3(0, 180, 0));
		transform.Move(Vector3(0, 0, -20));

		fov = 60.0f;
	}
	~Camera();

	Transform* GetTransform() {
		return &transform;
	}

	float GetScale() {
		return tan(0.0174533f*(fov * 0.5f));;
	}
};

