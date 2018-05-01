#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "Color.h"

class Material
{
private:
	static std::vector<Material*>materials;

	Material(std::string _name, std::string hexD, std::string hexS, std::string hexA, int _alpha, int _ref, int _tra, float _delta) {

		name = _name;

		diffuse = new Color(Color::colorConverter(hexD, 2.5f*_alpha));
		specular = new Color(Color::colorConverter(hexS, 255));
		ambient = new Color(Color::colorConverter(hexA, 255));

		transparency = (float)_alpha/100.0f;

		reflex = (float)_ref / 100.0f;
		transm = (float)_tra / 100.0f;
		delta = _delta;
	}
	~Material();

	std::string name;

	Color* diffuse;
	Color* specular;
	Color* ambient;

	float reflex;
	float transm;
	float delta;
	float transparency;

public:

	static Material* LoadMaterial(std::string name) {

		if (GetMaterial(name) != nullptr) {
			return GetMaterial(name);
		}

		std::ifstream file; file.open(name + ".mat", std::ifstream::in);

		std::string hexDiffuse;
		std::string hexSpecular;
		std::string hexAmbient;

		int _transp = 0;
		int _reflex = 0;
		int _transm = 0;
		float _delta = 0;

		if (file.bad()) {
			file.close();
			return nullptr;
		}

		file >> hexDiffuse >> hexSpecular >> hexAmbient >> _transp >> _reflex >> _transm >> _delta;

		file.close();

		materials.push_back(new Material(name, hexDiffuse, hexSpecular, hexAmbient, _transp, _reflex, _transm, _delta));

		return materials.back();
	}

	static Material* GetMaterial(std::string name) {
		for (int i = 0; i < materials.size(); i++) {
			if (materials[i]->GetName() == name) {
				return materials[i];
			}
		}
		return nullptr;
	}

	Color* GetDiffuse() {
		return diffuse;
	}

	Color* GetSpecular() {
		return specular;
	}

	Color* GetAmbient() {
		return ambient;
	}

	float GetReflex() {
		return reflex;
	}

	float GetTransparency() {
		return transparency;
	}

	float GetTransm() {
		return transm;
	}

	float GetRefractionConstant() {
		return delta;
	}

	std::string GetName() {
		return name;
	}
};

