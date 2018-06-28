#pragma once
#include "Image.h"
#include <iostream>
#include "glm/glm/gtc/noise.hpp"

class Filter
{
private:

	float*f;
	int height;
	int width;

	float factor;
	float bias;

	Filter();
	Filter(float* matrix, float _factor, float _bias, int w, int h) {

		f = matrix;
		height = h;
		width = w;

		factor = _factor;
		bias = _bias;
	}

public:
	~Filter();

	//float* operator[](int i) {
	//	return f[i];
	//}

	float Get(int x, int y) {
		return f[width*y+x];
	}

	int GetHeight() {
		return height;
	}

	int GetWidth() {
		return width;
	}

	float GetFactor() {
		return factor;
	}

	float GetBias() {
		return bias;
	}

	static Filter Blur() {
		float* i = new float[25]
		{
			0, 0, 1, 0, 0,
			0, 1, 1, 1, 0,
			1, 1, 1, 1, 1,
			0, 1, 1, 1, 0,
			0, 0, 1, 0, 0,
		};

		return Filter(i, 1.0f/13.0f, 0, 5, 5);
	}

	static Filter SobelGx() {
		float* i = new float[9]
		{
			-1, 0, 1,
			-2, 0, 2,
			-1, 0, 1
		};

		return Filter(i, 1.0f, 0, 3, 3);
	}
	static Filter SobelGy() {
		float* i = new float[9]
		{
			1, 2, 1,
			0, 0, 0,
			-1, -2, -1
		};

		return Filter(i, 1.0f, 0, 3, 3);
	}

	static Filter MotionBlur() {
		float* i = new float[9*9]
		{
			1, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 1, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 1, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 1, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 1, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 1, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 1, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 1, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 1,
		};

		return Filter(i, 1.0f / 9.0f, 0, 9, 9);
	}

	static void ApplyFilter(Image* img, Image& out, Filter filter) {
		
		for (int x = 0; x < img->getWidth(); x++) {
			for (int y = 0; y < img->getHeight(); y++)
			{
				int red = 0.0f, green = 0.0f, blue = 0.0f;

				if (x == img->getWidth() / 2 && y == img->getHeight() / 2) {
					red = red;
				}

				//multiply every value of the filter with corresponding image pixel
				for (int filterY = 0; filterY < filter.GetHeight(); filterY++) {
					for (int filterX = 0; filterX < filter.GetWidth(); filterX++)
					{
						int imageX = (x - filter.GetWidth() / 2 + filterX + img->getWidth()) % img->getWidth();
						int imageY = (y - filter.GetHeight() / 2 + filterY + img->getHeight()) % img->getHeight();

						char* pixel = img->getPixel(imageX, imageY);

						red += ((int)pixel[2]) * filter.Get(filterX, filterY);
						green += ((int)pixel[1]) * filter.Get(filterX, filterY);
						blue += ((int)pixel[0]) * filter.Get(filterX, filterY);

						free(pixel);

						//filter[filterY][filterX];
					}
				}

				out.setPixel(Color(Vector3(red*filter.factor+filter.GetBias(),
					green*filter.factor + filter.GetBias(),
					blue*filter.factor + filter.GetBias()) / 255.0f, 1), x, y);

				//result[y * img->getWidth() + x].r = min(max(int(factor * red + bias), 0), 255);
				//result[y * img->getWidth() + x].g = min(max(int(factor * green + bias), 0), 255);
				//result[y * img->getWidth() + x].b = min(max(int(factor * blue + bias), 0), 255);
			}
		}

	}

	
	static void ApplySobel(Image* img, Image& out) {

		Image _out = &out;
		Filter soberX = SobelGx();
		Filter soberY = SobelGy();

		for (int x = 0; x < img->getWidth(); x++) {
			for (int y = 0; y < img->getHeight(); y++)
			{
				int red = 0.0f, green = 0.0f, blue = 0.0f;
				int _red = 0.0f, _green = 0.0f, _blue = 0.0f;

				for (int filterY = 0; filterY < soberX.GetHeight(); filterY++) {
					for (int filterX = 0; filterX < soberX.GetWidth(); filterX++)
					{
						int imageX = (x - soberX.GetWidth() / 2 + filterX + img->getWidth()) % img->getWidth();
						int imageY = (y - soberX.GetHeight() / 2 + filterY + img->getHeight()) % img->getHeight();

						char* pixel = img->getPixel(imageX, imageY);

						red += ((int)pixel[2]) * soberX.Get(filterX, filterY);
						green += ((int)pixel[1]) * soberX.Get(filterX, filterY);
						blue += ((int)pixel[0]) * soberX.Get(filterX, filterY);

						_red += ((int)pixel[2]) * soberY.Get(filterX, filterY);
						_green += ((int)pixel[1]) * soberY.Get(filterX, filterY);
						_blue += ((int)pixel[0]) * soberY.Get(filterX, filterY);

						free(pixel);
					}
				}

				_out.setPixel(Color(Vector3(sqrtf(powf(red, 2)+ powf(_red, 2)),
					sqrtf(powf(green, 2) + powf(_green, 2)),
					sqrtf(powf(blue, 2) + powf(_blue, 2))) / 255.0f, 1), x, y);
			}
		}

		out = &_out;

	}

	static void ApplyGrayscale(Image* img, Image& out, Vector3 f = Vector3(1, 1, 1), bool average = true) {

		Image _out = &out;

		for (int x = 0; x < img->getWidth(); x++) {
			for (int y = 0; y < img->getHeight(); y++)
			{

				Color c = img->GetPixel(x, y);
				float _c = (c.r*f.x + c.g*f.y + c.b*f.z) / (average? 3.0f : 1);

				_out.setPixel(Color(_c, _c, _c, c.a), x, y);
			}
		}

		out = &_out;
	}

	static void ApplyInvert(Image* img, Image& out) {

		Image _out = &out;

		for (int x = 0; x < img->getWidth(); x++) {
			for (int y = 0; y < img->getHeight(); y++)
			{
				Color c = img->GetPixel(x, y);
				_out.setPixel(Color(1.0f-c.r, 1.0f - c.g, 1.0f - c.b, c.a), x, y);
			}
		}

		out = &_out;
	}

	static void ApplyBinary(Image* img, Image& out, float f = 0.5f) {

		Image _out = &out;

		for (int x = 0; x < img->getWidth(); x++) {
			for (int y = 0; y < img->getHeight(); y++)
			{
				Color c = img->GetPixel(x, y);

				float _c = (c.r > f) ? 1.0f : 0.0f;
				_out.setPixel(Color(_c, _c, _c, c.a), x, y);
			}
		}

		out = &_out;
	}

	static void CreateMask(Image* img, Image& out, Color maskColor = Color()) {

		Image _out = &out;

		for (int x = 0; x < img->getWidth(); x++) {
			for (int y = 0; y < img->getHeight(); y++)
			{
				Color c = img->GetPixel(x, y);
				if (c == maskColor) {
					_out.setPixel(c>>0, x, y);
				}
				else {
					_out.setPixel(c, x, y);
				}
			}
		}

		out = &_out;
	}
	
	static void ApplyPerlinNoise(Image* img, double scale = 1.0) { //scale >= 1
		scale = (scale < 1.0f) ? 1.0f : scale;

		for (int x = 0; x < img->getWidth(); x++) {
			for (int y = 0; y < img->getHeight(); y++) {
				double nx = (double)x / img->getWidth() - 0.5, ny = (double)y / img->getHeight() - 0.5;
				double f = (1.0 / scale) * glm::perlin(glm::vec2(scale * nx, scale * ny));

				Color c = img->GetPixel(x, y) * f;
				img->setPixel(c, x, y);
			}
		}
	}

	static void ApplyToneMapping(Image* img, Image& out, float a, int scale = 1) { //scale >= 1 && scale % 2 = 0
		a = (a > 1.0f) ? 1.0f : a;
		scale = (scale < 1) ? 1 : scale;
		//scale = (scale % 2 != 0) ? 1 : scale;

		Image _out = &out;

		Image ilumination = Image(img->getWidth(), img->getHeight());
		Image::CreateIlluminationMap(img, ilumination, scale);

		ilumination.ExportBMP("ilum " + std::to_string(scale));

		for (int x = 0; x < img->getWidth(); x++) {
			for (int y = 0; y < img->getHeight(); y++) {
				//Color c = (img->GetPixel(x, y) * (a / ilumination.GetPixel(x/scale, y/scale).r))+ img->GetPixel(x, y);

				Color c = (img->GetPixel(x, y) * (a / ilumination.GetPixel(x, y).r)) + img->GetPixel(x, y);
				_out.setPixel(c, x, y);
			}
		}

		out = &_out;
	}

	static void ApplyMyToneMapping(Image* img, Image& out, float a, int scale = 1) { //scale >= 1 && scale % 2 = 0
		a = (a > 1.0f) ? 1.0f : a;
		scale = (scale < 1) ? 1 : scale;
		//scale = (scale % 2 != 0) ? 1 : scale;

		Image _out = &out;

		Image ilumination = Image(img->getWidth() / scale, img->getHeight() / scale);
		Image::CreateIlluminationMap(img, ilumination, scale);
		//Filter::ApplyInvert(&ilumination, ilumination);

		ilumination.ExportBMP("ilum2 " + std::to_string(scale));

		for (int x = 0; x < img->getWidth(); x++) {
			for (int y = 0; y < img->getHeight(); y++) {

				float v = ilumination.GetPixel(x / scale, y / scale).r;
				float vo = (v*2) / (v);

				Color c = (img->GetPixel(x, y) * (a / vo)) + img->GetPixel(x, y);

				_out.setPixel(c, x, y);
			}
		}

		out = &_out;
	}
	
};

