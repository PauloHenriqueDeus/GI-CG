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

	float power;

	static std::vector<Light*>lights;
public:
	
	Light(Vector3 _position, float _power) {
		position = _position;
		power = _power;

		lights.push_back(this);
	}

	static Color PixelColor(Ray* ray, Vector3* surfacePoint, Vector3* surfaceNormal, int objectId) {
		Vector3 normal_eye = ray->GetDirection()->Normalized();
		Color light = Color(0.0f, 0.0f, 0.0f, 1.0f);


		float shadow = 1;
		for (int i = 0; i < lights.size(); i++) {
			Vector3 L = (lights[i]->position - *surfacePoint);

			//shadow
			for (int o = 0; o < Object::GetObjectsLenght(); o++)
			{
				if (o != objectId) {
					Vector3 hit;
					if (Object::GetObjects()[o]->RayCast(new Ray(*surfacePoint, lights[i]->position - (*surfacePoint)), hit)) {
						shadow *= (Object::GetObjects()[o]->GetMaterial()->GetTransparency());
					}
				}
			}

			/*if (!shadow)*/ {
				//sem sombra

				float fatt = 1.0f / ( /*0.0f +
					(0.0f*L.Magnitude())+*/
					(0.0002f*L.Magnitude()*L.Magnitude()));

				L = L.Normalized();
				Vector3 V = (*ray->GetOrigin() - *surfacePoint).Normalized();
				Vector3 N = (*surfaceNormal).Normalized();

				Color Ia = Color(lights[i]->La * Object::GetObjects()[objectId]->GetMaterial()->GetAmbient()->ToVector3()/*Ka*/, 1.0f);

				float dot = Vector3::Dot(N, L);
				dot = Max(dot, 0);
				Color Id = Color(lights[i]->Ld * (Object::GetObjects()[objectId]->GetMaterial()->GetDiffuse()->ToVector3())/*Kd*/ * dot, 1.0f);

				Vector3 R = (-L).Reflection(&N);
				float RV = Max(0, Vector3::Dot(V, R));

				float shinnessFactor = pow(Object::GetObjects()[objectId]->GetMaterial()->GetReflex(), 3);
				Color Is = Color(lights[i]->Ls * Object::GetObjects()[objectId]->GetMaterial()->GetSpecular()->ToVector3() /*Ks*/ *
					powf(RV, (shinnessFactor*0.98f + 0.02f) *1000), 1.0f) * (shinnessFactor*0.8f+0.2f);//shininess

				//Is.Clamp();

				//Is = Color(Is.ToVector3()*fatt, 1.0f);


				Color final = ((Ia + (Id + Is)*fatt)*shadow) >> Object::GetObjects()[objectId]->GetMaterial()->GetTransparency();

				light = light + final;

				//texture
				//vec4 texel = texture(basic_texture, texture_coordinates);

				// final colour
				//fragment_colour = texel; //mix(light, texel, 0.5f);
			}

		}

		return light;
	}

	static float Max(float a, float b) {
		return (a > b) ? a : b;
	}

	static float Min(float a, float b) {
		return (a < b) ? a : b;
	}

	

	~Light();
};

