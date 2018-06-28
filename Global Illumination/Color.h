#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "Vector3.h"
#include <string>
class Color
{
public:
	float r;
	float g;
	float b;
	float a;

	Color();
	Color(float _r, float _g, float _b, float _a) {
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}
	Color(int _r, int _g, int _b, int _a) {
		r = (float)_r / 255;
		g = (float)_g / 255;
		b = (float)_b / 255;
		a = (float)_a / 255;
	}
	Color(Vector3 rgb, float _a) {
		r = rgb.x;
		g = rgb.y;
		b = rgb.z;
		a = _a;

		Clamp();
	}
	~Color();

	Vector3 ToVector3() {
		return Vector3(r, g, b);
	}

	Color operator+(const Color& c) {

		Color r = *this;

		r.r = r.r * r.a + c.r * c.a;
		r.g = r.g * r.a + c.g * c.a;
		r.b = r.b * r.a + c.b * c.a;

		r.Clamp();

		return r;
	}

	Color& operator=(Vector3 v) {
		this->r = v.x;
		this->g = v.y;
		this->b = v.z;
		return *this;
	}

	Color operator* (Color& c) {
		Color _r;

		_r.r = this->r * c.r;
		_r.g = this->g * c.g;
		_r.b = this->b * c.b;

		_r.Clamp();

		return _r;
	}

	void Clamp() {

		r = (r < 0.0f) ? 0.0f : r;
		g = (g < 0.0f) ? 0.0f : g;
		b = (b < 0.0f) ? 0.0f : b;
		a = (a < 0.0f) ? 0.0f : a;

		r = (r > 1.0f) ? 1.0f : r;
		g = (g > 1.0f) ? 1.0f : g;
		b = (b > 1.0f) ? 1.0f : b;
		a = (a > 1.0f) ? 1.0f : a;

	}

	bool operator==(Color& c) {
		return (r == c.r && g == c.g && b == c.b);
	}

	static Color Avarage(Color a, Color b) {

		Color c;

		c.r = (a.r + b.r)/2;
		c.g = (a.g + b.g)/2;
		c.b = (a.b + b.b)/2;

		c.a = (a.a > b.a) ? a.a : b.a;

		return c;

	}

	static Color Lerp(Color a, Color b, float t) {
		Color c;

		c.r = (1 - t) * a.r + t * b.r;
		c.g = (1 - t) * a.g + t * b.g;
		c.b = (1 - t) * a.b + t * b.b;

		c.a = (a.a > b.a) ? a.a : b.a;

		return c;
	}

	static Color colorConverter(std::string hexValue, int alpha)
	{
		int r, g, b;
		sscanf_s(hexValue.c_str(), "%02x%02x%02x", &r, &g, &b);

		return Color(r, g, b, alpha);
	}

	Color& operator>> (const float& f) {
		this->a = f;
		return *this;
	}

	Color operator* (float f) {
		if (f < 0) {
			//f = (-f);
			f = 0;
		}
		f = (f > 1) ? 1 : f;

		Color c = *this;
		c.r = c.r*f;
		c.g = c.g*f;
		c.b = c.b*f;
		return c;
	}

	static Color White() {
		return Color(1.f, 1.f, 1.f, 1.f);
	}
};

