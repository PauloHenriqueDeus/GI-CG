#pragma once
#include <windows.h>
#include <math.h>

#include "Image.h"
#include "Color.h"
#include "Camera.h"
#include "Ray.h"
#include "Object.h"
#include "Light.h"
#include "Filter.h"

class Renderer
{
private:
	int windowW;
	int windowH;

	bool npr;

	Camera camera;
	Image *display;
	Image *nprStroke;

	static Renderer* renderer;
public:
	Renderer(int x, int y) {
		windowW = x;
		windowH = y;
		renderer = this;
	}
	~Renderer();

	Image* GetDisplay() {
		return display;
	}

	static void SetCamera(Camera c) {
		renderer->camera = c;
	}

	bool Initialize() {

		display = new Image(windowW, windowH);
		nprStroke = new Image(windowW, windowH);

		return true;
	}

	void ClearBuffer() {
		free(display);
	}

	void Render() {
		float aspectRatio = (float)display->getWidth() / display->getHeight();
		Vector3 origem = *camera.GetTransform()->GetPosition();

		Ray* ray;
		Color nprMask;

		for (int i = 0; i < display->getWidth(); i++) {
			for (int j = 0; j < display->getHeight(); j++) {
				float x = (2 * (i + 0.5) / (float)display->getWidth() - 1) * aspectRatio * camera.GetScale();
				float y = (1 - 2 * (j + 0.5) / (float)display->getHeight()) * camera.GetScale();

				Vector3 dir = (Vector3(x, y, -1)).Normalized();

				ray = new Ray(*camera.GetTransform()->GetPosition(), dir*100.0f);


				display->setPixel(RayTracing(ray, 2, 1.0f, &nprMask), i, j);
				nprStroke->setPixel(nprMask, i, j);
				nprMask = Color();
				delete ray;
			}
		}

		if (NPR())
		{
			Image stroke = nprStroke;
			Filter::ApplySobel(&stroke, stroke);
			Filter::ApplyGrayscale(&stroke, stroke);

			Image perlin = Image(display->getWidth(), display->getHeight());
			Image::PerlinNoisePre(&perlin, 60);
			Filter::ApplyBinary(&perlin, perlin, 0.6f);
			//Filter::ApplyInvert(&perlin, perlin);

			Image::Blend(&stroke, &perlin, Image::BlendMode::Multiply);

			Filter::ApplyBinary(&stroke, stroke, 0.2f);
			stroke *= Color(1.f, 1.f, 1.f, 1.f);

			Filter::CreateMask(&stroke, stroke);

			perlin = Image(display->getWidth(), display->getHeight());
			Image::PerlinNoisePre(&perlin, 80);

			//Filter::ApplyInvert(&perlin, perlin);
			Image::Blend(display, &perlin, Image::BlendMode::Multiply);
			//perlin.ExportBMP("perlin");

			perlin.ExportBMP("perlin");


			Image::Blend(display, &stroke, Image::BlendMode::Normal);

			stroke.ExportBMP("mask");
		}
		else {

			Image hdr = display;
			Filter::ApplyToneMapping(display, hdr, .72f, 1);
			hdr.InvertY();
			hdr.ExportBMP("hdr scale 1 px");
		}


		display->InvertY();


		nprStroke->InvertY();
		ExportImage();
	}

	void ExportImage();

	Color RayTracing(Ray* ray, int depht, float refraction, Color* NPRMask) {

		Color pixel;

		if (depht > 0) {
			depht--;

			float d = 0;

			for (int o = 0; o < Object::GetObjectsLenght(); o++)
			{
				Vector3 hit;
				if (Object::GetObjects()->at(o)->RayCast(ray, hit)) {

					float od = (hit - (*ray->GetOrigin())).Magnitude();
					if (od < d || d == 0) {
						if (od >= 0.1f) { //t < 0 -> t +/- 2.xx

							d = (hit - (*ray->GetOrigin())).Magnitude();


							Vector3 n = Object::GetObjects()->at(o)->Normal(hit).Normalized();

							Color rt;

							float reflex = Object::GetObjects()->at(o)->GetMaterial()->GetReflex();
							if (reflex > 0) {
								Vector3 ran = Vector3(powf(float(rand() % 100) / 100, 2), powf(float(rand() % 100) / 100, 2), powf(float(rand() % 100) / 100, 2)) * (1 - reflex)*.5f
									+ (ray->GetDirection()->Normalized().Reflection(&n));

								Ray* r = new Ray(hit, (ran).Normalized()*ray->GetSize());
								rt = rt + (RayTracing(r, depht, refraction, NPRMask) >> reflex);
								delete r;
							}

							float refr = Object::GetObjects()->at(o)->GetMaterial()->GetRefractionConstant();
							if (Object::GetObjects()->at(o)->GetMaterial()->GetTransm() > 0) {
								Ray* r = new Ray(hit, (ray->GetDirection()->Normalized().Refraction(&n, refraction, refr) *ray->GetSize()));
								rt = rt + (RayTracing(r, depht, refraction, NPRMask) >> Object::GetObjects()->at(o)->GetMaterial()->GetTransm());
								delete r;
							}


							pixel = (Light::PixelColor(ray, &hit, &n, o, NPRMask) + rt);
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

	static void SetNPR(bool _b) {
		renderer->npr = _b;
	}

	static bool NPR() {
		return renderer->npr;
	}

	static Image* NPRImage() {
		return renderer->nprStroke;
	}

	static void RenderImage(Image* image) {

		for (int i = 0; i < renderer->display->getWidth() && i < image->getWidth(); i++) {
			for (int j = 0; j < renderer->display->getHeight() && j < image->getHeight(); j++) {
				renderer->display->setPixel(image->GetPixel(i, j), i, j);
			}
		}
	}
};

