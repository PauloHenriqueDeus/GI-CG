#include "Scene.h"


std::vector<Scene*> Scene::scenes = std::vector<Scene*>();


Scene::~Scene()
{
	for (int i = 0; i < objects.size(); i++) {
		free(objects[i]);
	}

	for (int i = 0; i < lights.size(); i++) {
		free(lights[i]);
	}

	objects.clear();
	lights.clear();
}
