#pragma once

#include <fstream>
#include <string>

#include "Sphere.h"
#include "Light.h"
#include "Cube.h"
#include "Camera.h"
#include "Renderer.h"

#include <vector>

class Scene
{
private:

	static std::vector<Scene*>scenes;

	std::vector<Object*>objects;
	std::vector<Light*>lights;

	std::string name;

	Scene(std::string _name) {
		name = _name;
		//scenes.push_back(this);
	}
	~Scene();
public:


	std::vector<Object*>* Objects() {
		return &objects;
	}

	std::vector<Light*>* Lights() {
		return &lights;
	}

	std::string GetName() {
		return name;
	}

	static Scene* CurrentScene() {
		if (scenes.size() > 0) {
			return scenes[scenes.size()-1];
		}

		return nullptr;
	}

	static bool NextScene() {
		if (scenes.size() >= 2) {
			return UnloadScene();
		}
		return false;
	}

	static bool UnloadScene() {
		if (scenes.size() > 0) {
			free(scenes[scenes.size() - 1]);
			scenes.pop_back();
			return true;
		}
		return false;
	}

	static void LoadScene(std::string name) {
		scenes.push_back(new Scene(name));
		std::cout << "Loading scene '" << name << ".scene'..." << std::endl;

		std::ifstream file; file.open(name + ".scene", std::ios::in);

		if (file.bad()) {
			file.close();
			return;
		}

		std::string s;


		while (!file.eof()) {
			file >> s;

			if (s == "camera") {
				Vector3 p;
				Vector3 r;
				float f;

				file >> p.x >> p.y >> p.z >> r.x >> r.y >> r.z >> f;
				Renderer::SetCamera(Camera(p, r, f));
			}
			else if (s == "sphere") {
				Vector3 p;
				Vector3 r;
				float sc;
				std::string m;

				file >> p.x >> p.y >> p.z >> r.x >> r.y >> r.z >> sc >> m;
				CurrentScene()->objects.push_back(  new Sphere(p, r, sc, Material::LoadMaterial(m)));
			}
			else if (s == "plane") {
				Vector3 p;
				Vector3 r;
				Vector3 sc;
				std::string m;
				sc.z = 1;

				file >> p.x >> p.y >> p.z >> r.x >> r.y >> r.z >> sc.x >> sc.y >> m;
				CurrentScene()->objects.push_back(new AACube(p, r, sc, Material::LoadMaterial(m)));
			}
			else if (s == "light") {
				Vector3 p;
				Vector3 c;

				file >> p.x >> p.y >> p.z >> c.x >> c.y >> c.z;
				CurrentScene()->lights.push_back( new Light(p, c, c, Vector3(0,0,0)));
			}
			else if (s == "npr") {
				std::string b;

				file >> b;
				Renderer::SetNPR(b=="true");
			}
			
		}

		file.close();

	}

	static void LoadSceneList(std::string name) {
		std::ifstream file; file.open(name + ".scl", std::ios::in);

		if (file.bad()) {
			file.close();
			return;
		}

		std::string s;

		while (!file.eof()) {
			file >> s;
			LoadScene(s);
		}

		file.close();
	}
};

