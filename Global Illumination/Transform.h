#pragma once
#include "Vector3.h"


class Transform
{
private:
	Vector3 position;
	Vector3 scale;
	Vector3 rotation;

public:
	Transform();
	~Transform();

	Vector3* GetPosition();
	Vector3* GetScale();
	Vector3* GetRotation();

	void SetPosition(Vector3 _pos) {
		position = _pos;
	}
	void SetScale(Vector3 _scale) {
		scale = _scale;
	}
	void SetRotation(Vector3 _deg) {
		rotation = _deg;
	}

	//mat4 GetModelMatrix() {
	//	mat4 R = rotate_x_deg(identity_mat4(), rotation.x) * rotate_y_deg(identity_mat4(), rotation.y) * rotate_z_deg(identity_mat4(), rotation.z);
	//	mat4 P = translate(identity_mat4(), vec3(position.x, position.y, position.z));
	//	//mat4 S = scalev(identity_mat4(), vec3(scale.x, scale.y, scale.z));
	//	return R*P;
	//}

	void Rotate(Vector3 angle) {

		rotation.x += angle.x;
		if (rotation.x >= 360) rotation.x -= 360;
		if (rotation.x <= -360) rotation.x += 360;

		rotation.y += angle.y;
		if (rotation.y >= 360) rotation.y -= 360;
		if (rotation.y <= -360) rotation.y += 360;

		rotation.z += angle.z;
		if (rotation.z >= 360) rotation.z -= 360;
		if (rotation.z <= -360) rotation.z += 360;

		
	}

	void Move(Vector3 o) {
		position += o;
	}

	Vector3 GetForward() {

		float x = sinf(rotation.y * 0.0174533f);
		float z = cosf(rotation.y * 0.0174533f);

		return Vector3(x, 0, z);
	}

	Vector3 GetRight() {

		float x = sinf(rotation.y * 0.0174533f);
		float z = cosf(rotation.y * 0.0174533f);

		return Vector3(z, 0, x);
	}

	Vector3 GetUp() {

		float z = sinf(rotation.x * 0.0174533f);
		float y = cosf(rotation.x * 0.0174533f);

		return Vector3(0, y, z);
	}

};

