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
	}
	~Color();

	Vector3 ToVector3() {
		return Vector3(r, g, b);
	}

	Color operator+(const Color& c) {

		Color r = *this;

		r.r = r.r * r.a + c.r * (c.a);
		r.g = r.g * r.a + c.g * (c.a);
		r.b = r.b * r.a + c.b * (c.a);

		r.Clamp();

		return r;
	}

	Color& operator=(Vector3 v) {
		this->r = v.x;
		this->g = v.y;
		this->b = v.z;
		return *this;
	}

	void Clamp() {
		r = (r > 1.0f) ? 1.0f : r;
		g = (g > 1.0f) ? 1.0f : g;
		b = (b > 1.0f) ? 1.0f : b;
		a = (a > 1.0f) ? 1.0f : a;
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
		f = (f > 1) ? 1 : f;
		Color c = *this;
		c.r = c.r*f;
		c.g = c.g*f;
		c.b = c.b*f;
		return c;
	}
};

