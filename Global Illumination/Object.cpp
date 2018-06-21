#include "Object.h"
#include "Scene.h"


Object::Object(Vector3 position, Vector3 rotation, Material * _mat)
{
	transform.Move(position);
	transform.Rotate(rotation);

	//Scene::CurrentScene()->Objects()->push_back(this);
	mat = _mat;
}

Object::~Object()
{
}

std::vector<Object*>* Object::GetObjects()
{
	return Scene::CurrentScene()->Objects();
}

int Object::GetObjectsLenght()
{
	return Scene::CurrentScene()->Objects()->size();
}

