#pragma once
#include <windows.h>
#include <math.h>

#include "Image.h"
#include "Color.h"
#include "Camera.h"
#include "Ray.h"
#include "Object.h"
#include "Light.h"

class Renderer
{
private:
	int windowW;
	int windowH;

	Camera camera;

	Image *display;
public:
	Renderer(int x, int y) {
		windowW = x;
		windowH = y;
	}
	~Renderer();

	Image* GetDisplay() {
		return display;
	}

	bool Initialize() {

		display = new Image(windowW, windowH);

		/*for (int i = 0; i < display->getWidth(); i++) {
			for (int j = 0; j < display->getHeight(); j++) {
				display->setPixel(Color(
					(i < display->getWidth() / 2) ? 1.0f - (2 * (float)i / display->getWidth()) : 0.0f,
					(i < display->getWidth() / 2) ? 0.0f : 1.0f - (2 - 2 * (float)i / display->getWidth()),
					(i < display->getWidth() / 2) ? 2 * (float)i / display->getWidth() : 2 - 2 * (float)i / display->getWidth(),
					1.0f), i, j);
			}
		}*/

		for (int i = 0; i < display->getWidth(); i++) {
			for (int j = 0; j < display->getHeight(); j++) {
				display->setPixel(Color(0.0f, 0.0f, 0.0f, 1.0f), i, j);
			}
		}

		return true;
	}

	void Render() {

		float aspectRatio = (float)display->getWidth() / display->getHeight();
		Vector3 origem = *camera.GetTransform()->GetPosition();

		Ray* ray;

		for (int i = 0; i < display->getWidth(); i++) {
			for (int j = 0; j < display->getHeight(); j++) {
				float x = (2 * (i + 0.5) / (float)display->getWidth() - 1) * aspectRatio * camera.GetScale();
				float y = (1 - 2 * (j + 0.5) / (float)display->getHeight()) * camera.GetScale();

				Vector3 dir = (Vector3(x, y, -1)).Normalized();

				ray = new Ray(*camera.GetTransform()->GetPosition(), dir*100.0f);
				display->setPixel(RayTracing(ray, 2, 1.0f), i, j);
				delete ray;
			}
		}
		display->InvertY();

	}

	Color RayTracing(Ray* ray, int depht, float refraction) {

		Color pixel;

		if (depht > 0) {
			depht--;

			float d = 0;

			for (int o = 0; o < Object::GetObjectsLenght(); o++)
			{
				Vector3 hit;
				if (Object::GetObjects()[o]->RayCast(ray, hit)) {

					float od = (hit - (*ray->GetOrigin())).Magnitude();
					if (od < d || d == 0) {
						if (od >= 0.1f) { //t < 0 -> t +/- 2.xx

							d = (hit - (*ray->GetOrigin())).Magnitude();


							Vector3 n = Object::GetObjects()[o]->Normal(hit).Normalized();

							Color rt;

							float reflex = Object::GetObjects()[o]->GetMaterial()->GetReflex();
							if (reflex > 0) {
								Vector3 ran = Vector3(powf(float(rand() % 100) / 100, 2), powf(float(rand() % 100) / 100, 2), powf(float(rand() % 100) / 100, 2)) * (1-reflex)*.5f
									+(ray->GetDirection()->Normalized().Reflection(&n));

								Ray* r = new Ray(hit, (ran).Normalized()*ray->GetSize());
								rt = rt + (RayTracing(r, depht, refraction) >> reflex);
								delete r;
							}

							float refr = Object::GetObjects()[o]->GetMaterial()->GetRefractionConstant();
							if (Object::GetObjects()[o]->GetMaterial()->GetTransm() > 0) {
								Ray* r = new Ray(hit, (ray->GetDirection()->Normalized().Refraction(&n, refraction, refr) *ray->GetSize()));
								rt = rt + (RayTracing(r, depht, refraction) >> Object::GetObjects()[o]->GetMaterial()->GetTransm());
								delete r;
							}

							pixel =  (Light::PixelColor(ray, &hit, &n, o) + rt);
						}
					}

				}
			}

			return pixel;

		}
		else {
			return pixel;
		}

	}


};

