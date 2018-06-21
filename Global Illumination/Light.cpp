#include "Light.h"
#include "Scene.h"


Light::Light(Vector3 _position, Vector3 ls, Vector3 ld, Vector3 la)
{
	Ls = ls;
	Ld = ld;
	La = la;

	position = _position;

	//Scene::CurrentScene()->Lights()->push_back(this);
}

Light::Light(Vector3 _position)
{
	position = _position;

	Scene::CurrentScene()->Lights()->push_back(this);
}

Color Light::PixelColor(Ray * ray, Vector3 * surfacePoint, Vector3 * surfaceNormal, int objectId, Color* NPRMask)
{
	Vector3 normal_eye = ray->GetDirection()->Normalized();
	Color light = Color(0.0f, 0.0f, 0.0f, 1.0f);


	float shadow = 1;
	for (int i = 0; i < Scene::CurrentScene()->Lights()->size(); i++) {
		Vector3 L = (Scene::CurrentScene()->Lights()->at(i)->position - *surfacePoint);

		//shadow
		for (int o = 0; o < Object::GetObjectsLenght(); o++)
		{
			if (o != objectId) {
				Vector3 hit;
				Ray* r = new Ray(*surfacePoint, Scene::CurrentScene()->Lights()->at(i)->position - (*surfacePoint));
				if (Object::GetObjects()->at(o)->RayCast(r, hit)) {
					shadow *= (1 - Object::GetObjects()->at(o)->GetMaterial()->GetTransparency());
				}
				delete r;
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


			Color Ia = Color(Scene::CurrentScene()->Lights()->at(i)->La * Object::GetObjects()->at(objectId)->GetMaterial()->GetAmbient()->ToVector3()/*Ka*/, 1.0f);

			float dot = Vector3::Dot(N, L);
			dot = Max(dot, 0);

			Color Id;

			if (Renderer::NPR()) {
				if (dot >= 0.5f) {
					Id = Color(Scene::CurrentScene()->Lights()->at(i)->Ld * Object::GetObjects()->at(objectId)->GetMaterial()->GetDiffuse()->ToVector3(), 1.0f);
				}
				else
					Id = Color(Scene::CurrentScene()->Lights()->at(i)->Ld * Object::GetObjects()->at(objectId)->GetMaterial()->GetDiffuse()->ToVector3() * 0.5f, 1.0f);
			
				*NPRMask = Color::Avarage(*NPRMask, *Object::GetObjects()->at(objectId)->GetMaterial()->GetDiffuse());

				//*NPRMask = *NPRMask + *Object::GetObjects()->at(objectId)->GetMaterial()->GetDiffuse();
			}
			else {
				Id = Color(Scene::CurrentScene()->Lights()->at(i)->Ld * Object::GetObjects()->at(objectId)->GetMaterial()->GetDiffuse()->ToVector3()/*Kd*/ * dot, 1.0f);
			}

			Vector3 R = (-L).Reflection(&N);
			float RV = Max(0, Vector3::Dot(V, R));

			float shinnessFactor = pow(Object::GetObjects()->at(objectId)->GetMaterial()->GetReflex(), 3);
			
			Color Is = Color(Scene::CurrentScene()->Lights()->at(i)->Ls * Object::GetObjects()->at(objectId)->GetMaterial()->GetSpecular()->ToVector3() /*Ks*/ *
				powf(RV, (shinnessFactor*0.98f + 0.02f) * 1000), 1.0f) * (shinnessFactor*0.8f + 0.2f);

			Color final = (Ia + (Id + Is)*fatt*shadow) >> Object::GetObjects()->at(objectId)->GetMaterial()->GetTransparency();

			light = light + final;

			//texture
			//vec4 texel = texture(basic_texture, texture_coordinates);

			// final colour
			//fragment_colour = texel; //mix(light, texel, 0.5f);
		}

	}

	return light;
}

Light::~Light()
{
}
