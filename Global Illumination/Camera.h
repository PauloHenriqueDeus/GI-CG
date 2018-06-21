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
	Camera(Vector3 pos, Vector3 rot, float _fov) {
		transform.Rotate(Vector3(0, 180, 0));
		transform.Move(pos);
		transform.Rotate(rot);

		fov = _fov;
	}
	~Camera();

	Transform* GetTransform() {
		return &transform;
	}

	float GetScale() {
		return tan(0.0174533f*(fov * 0.5f));;
	}
};

