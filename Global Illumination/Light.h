#pragma once
#include "Transform.h"
#include "Color.h"
#include "Object.h"
#include <vector>
#include <cmath>

class Light
{
private:
	Vector3 position;
	Vector3 Ls = Vector3(1.0, 1.0, 1.0); // white specular colour
	Vector3 Ld = Vector3(0.7, 0.7, 0.7); // dull white diffuse light colour
	Vector3 La = Vector3(0.0, 0.0, 0.0); // grey ambient colour

	//static std::vector<Light*>lights;
public:
	
	Light(Vector3 _position, Vector3 ls, Vector3 ld, Vector3 la);
	Light(Vector3 _position);

	static Color PixelColor(Ray* ray, Vector3* surfacePoint, Vector3* surfaceNormal, int objectId, Color* NPRMask);

	static float Max(float a, float b) {
		return (a > b) ? a : b;
	}

	static float Min(float a, float b) {
		return (a < b) ? a : b;
	}

	

	~Light();
};

