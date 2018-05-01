#include "Transform.h"



Transform::Transform()
{
	scale = Vector3(1, 1, 1);
}


Transform::~Transform()
{
}

Vector3 * Transform::GetPosition()
{
	return &position;
}

Vector3 * Transform::GetScale()
{
	return &scale;
}

Vector3 * Transform::GetRotation()
{
	return &rotation;
}
