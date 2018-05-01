#pragma once
#include "Color.h"

class Image {
public:
	Image(int w, int h) {
		channels = 4;
		width = w; height = h; this->channels = channels;
		pixels = new unsigned char[w*h*channels];
	}

	void setPixel(char *rgb, int x, int y) {
		for (int i = 0; i < channels; i++)
			pixels[i + x*channels + y*width*channels] = rgb[i];
	}

	void setPixel(Color c, int x, int y) {
		
		char rgb[4] = {(int)(c.b*255), (int)(c.g*255), (int)(c.r*255), (int)(c.a*255)};

		for (int i = 0; i < channels; i++)
			pixels[i + x*channels + y*width*channels] = rgb[i];
	}

	char* getPixel(int x, int y) {
		char *rgb = new char[channels];
		for (int i = 0; i < channels; i++)
			rgb[i] = pixels[i + x*channels + y*width*channels];
		return rgb;
	}

	unsigned char* getPixels() {
		return pixels;
	}

	void InvertY() {
		int width_in_bytes = width * 4;
		unsigned char *top = NULL;
		unsigned char *bottom = NULL;
		unsigned char temp = 0;
		int half_height = height / 2;

		for (int row = 0; row < half_height; row++) {
			top = pixels + row * width_in_bytes;
			bottom = pixels + (height - row - 1) * width_in_bytes;
			for (int col = 0; col < width_in_bytes; col++) {
				temp = *top;
				*top = *bottom;
				*bottom = temp;
				top++;
				bottom++;
			}
		}
	}

	int getWidth() { return width; }
	int getHeight() { return height; }
	int getNumOfChannels() { return channels; }
private:
	unsigned char *pixels;
	int width, height, channels;
};
