#pragma once
#include "Color.h"
#include "glm/glm/gtc/noise.hpp"
#include <iostream>

class Image {
public:
	Image(int w, int h) {
		channels = 4;
		width = w; height = h; this->channels = channels;
		pixels = new unsigned char[w*h*channels];

		/*pixels = (unsigned char*)malloc((w*h*channels));


		for (int i = 0; i < w*h*channels; i++) {
			pixels[i] = 255;
		}*/
	}

	Image(Image* image) {
		channels = 4;
		width = image->width; height = image->height; this->channels = channels;

		int size = width*height*channels;
		pixels = new unsigned char[size];

		for (int i = 0; i < size; i++) {
			pixels[i] = image->pixels[i];
		}

	}

	Image(char *filename) {
		FILE *file;
		unsigned long size;                 // size of the image in bytes.
		//char temp;

											// make sure the file is there.

		fopen_s(&file, filename, "rb");

		if (file) {

			fseek(file, 18, SEEK_CUR);

			{
				unsigned char  b[4];
				unsigned int i;

				if (fread(b, 1, 4, file) < 4)
					width = 0;

				i = (b[3] << 24) | (b[2] << 16) | (b[1] << 8) | b[0];
				width = i;
			}
			printf("Width of %s: %lu\n", filename, width);

			{
				unsigned char  b[4];
				unsigned int i;

				if (fread(b, 1, 4, file) < 4)
					height = 0;

				i = (b[3] << 24) | (b[2] << 16) | (b[1] << 8) | b[0];
				height = i;
			}
			printf("Height of %s: %lu\n", filename, height);

			size = width * height * 3; //4

			fseek(file, 4, SEEK_CUR);
			fseek(file, 24, SEEK_CUR);

			channels = 4;

			pixels = new unsigned char[width * height * 4];
			unsigned char* buffer = new unsigned char[size];

			fread_s(buffer, size, size, 1, file);

			int j = 0;
			for (int i = 0; i < size; i += 3) {
				/*temp = image->data[i];
				image->data[i] = image->data[i + 2];
				image->data[i + 2] = temp;*/

				pixels[j] = buffer[i];
				pixels[j + 1] = buffer[i + 1];
				pixels[j + 2] = buffer[i + 2];
				pixels[j + 3] = 255;

				j += 4;
			}
		}
	}

	~Image() {
		delete pixels;
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

	Color GetPixel(int x, int y) {
		unsigned r, g, b, a;
		r = pixels[0 + x * channels + y * width*channels];
		g = pixels[1 + x * channels + y * width*channels];
		b = pixels[2 + x * channels + y * width*channels];
		a = pixels[3 + x * channels + y * width*channels];

		Color c = Color((int)b, (int)g, (int)r, (int)a);
		return c;
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

	void ExportBMP(std::string name = "img") {
		FILE *f;
		unsigned char *img = NULL;
		int filesize = 54 + 3 * width*height;  //w is your image width, h is image height, both int

		img = (unsigned char *)malloc(3 * width*height);
		memset(img, 0, 3 * width*height);

		for (int i = 0; i<width; i++)
		{
			for (int j = 0; j<height; j++)
			{
				char *rgb = getPixel(i, j);

				int x = i; int y = (height - 1) - j;

				int r = rgb[2];
				int g = rgb[1];
				int b = rgb[0];

				//r = red[i][j] * 255;
				//g = green[i][j] * 255;
				//b = blue[i][j] * 255;

				if (r > 255) r = 255;
				if (g > 255) g = 255;
				if (b > 255) b = 255;

				img[(x + y*width) * 3 + 2] = (unsigned char)(r);
				img[(x + y*width) * 3 + 1] = (unsigned char)(g);
				img[(x + y*width) * 3 + 0] = (unsigned char)(b);

				free(rgb);
			}
		}

		unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
		unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
		unsigned char bmppad[3] = { 0,0,0 };

		bmpfileheader[2] = (unsigned char)(filesize);
		bmpfileheader[3] = (unsigned char)(filesize >> 8);
		bmpfileheader[4] = (unsigned char)(filesize >> 16);
		bmpfileheader[5] = (unsigned char)(filesize >> 24);

		bmpinfoheader[4] = (unsigned char)(width);
		bmpinfoheader[5] = (unsigned char)(width >> 8);
		bmpinfoheader[6] = (unsigned char)(width >> 16);
		bmpinfoheader[7] = (unsigned char)(width >> 24);
		bmpinfoheader[8] =  (unsigned char)(height);
		bmpinfoheader[9] =  (unsigned char)(height >> 8);
		bmpinfoheader[10] = (unsigned char)(height >> 16);
		bmpinfoheader[11] = (unsigned char)(height >> 24);

		name += ".bmp";
	
		fopen_s(&f, name.c_str(), "wb");
		
		fwrite(bmpfileheader, 1, 14, f);
		fwrite(bmpinfoheader, 1, 40, f);
		for (int i = 0; i<height; i++)
		{
			fwrite(img + (width*(height - i - 1) * 3), 3, width, f);
			fwrite(bmppad, 1, (4 - (width * 3) % 4) % 4, f);
		}

		free(img);
		fclose(f);
	}

	Image* operator= (Image* image) {
		channels = 4;
		width = image->width; height = image->height;

		int size = width*height*channels;
		delete pixels;

		pixels = new unsigned char[size];

		for (int i = 0; i < size; i++) {
			pixels[i] = image->pixels[i];
		}
		return this;
	}

	enum BlendMode{Normal, Multiply};

	static void Blend(Image* out, Image* top, BlendMode mode) {
		switch (mode) {
		case Normal:
			for (int x = 0; x < out->getWidth() && x < top->getWidth(); x++) {
				for (int y = 0; y < out->getHeight() && x < top->getWidth(); y++)
				{
					Color c = top->GetPixel(x, y);					
					out->setPixel(Color::Lerp(out->GetPixel(x, y), c, c.a), x, y);
				}
			}
			break;
		case Multiply:
			for (int x = 0; x < out->getWidth() && x < top->getWidth(); x++) {
				for (int y = 0; y < out->getHeight() && x < top->getWidth(); y++)
				{
					Color c = top->GetPixel(x, y);
					out->setPixel(out->GetPixel(x, y) * c, x, y);
				}
			}
			break;
		}

	}

	static void PerlinNoise(Image* img, float scale = 1.0f) { //scale >= 1
		scale = (scale < 1.0f) ? 1.0f : scale;

		for (int x = 0; x < img->getWidth(); x++) {
			for (int y = 0; y < img->getHeight(); y++) {
				double nx = (double)x / img->getWidth() - 0.5, ny = (double)y / img->getHeight() - 0.5;
				float f = (1.0f / scale) * glm::perlin(glm::vec2(scale * nx, scale * ny));

				Color c = Color::White() * f;
				img->setPixel(c, x, y);
			}
		}
	}

	static void PerlinNoisePre(Image* img, float scale = 1.0f) { //scale >= 1
		scale = (scale < 1.0f) ? 1.0f : scale;

		for (int x = 0; x < img->getWidth(); x++) {
			for (int y = 0; y < img->getHeight(); y++) {
				double nx = (double)x / img->getWidth(), ny = (double)y / img->getHeight();

				Color ca = (Color::White() * 0.5f * (1 - (1 - glm::simplex(glm::vec2(scale / 4 * nx, scale / 4 * ny))))) +
					(Color::White() * 0.5f * (1 - (1 - glm::simplex(glm::vec2(scale / 3 * nx, scale / 2 * ny))))) +
					(Color::White() * (1 - (1 - glm::simplex(glm::vec2(scale / 2 * nx, scale / 2 * ny))))) +
					(Color::White() * (1 - glm::simplex(glm::vec2(scale * nx, scale * ny))));

				float f = ca.r;
				f = powf(f, 2);
				ca = Color(f, f, f, 1.f);

				Color cb = (Color::White() * 0.5f * (1 - (1 - glm::simplex(glm::vec2(scale/2 / 4 * nx, scale/2 / 4 * ny))))) +
					(Color::White() * 0.5f * (1 - (1 - glm::simplex(glm::vec2(scale/2 / 3 * nx, scale/2 / 2 * ny))))) +
					(Color::White() * (1 - (1 - glm::simplex(glm::vec2(scale/2 / 2 * nx, scale/2 / 2 * ny))))) +
					(Color::White() * (1 - glm::simplex(glm::vec2(scale/2 * nx, scale/2 * ny))));

				//f = cb.r;
				//f = powf(f, 2);
				//cb = Color(f, f, f, 1.f);
				//
				img->setPixel(ca*cb, x, y);
			}
		}
	}

	Image* operator*= (Color& c) {
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++)
			{
				Color _c = GetPixel(x, y) * c;
				setPixel(_c, x, y);
			}
		}

		return this;
	}

	Image& operator=(Image& image) {
		channels = 4;
		width = image.width; height = image.height; this->channels = channels;

		int size = width*height*channels;
		pixels = new unsigned char[size];

		for (int i = 0; i < size; i++) {
			pixels[i] = image.pixels[i];
		}
		return *this;
	}

	void Scale(float scale) {

		Image* n = new Image(width*scale, height*scale);

		for (int x = 0; x < (int)(width*scale); x++) {
			for (int y = 0; y < (int)(height*scale); y++)
			{
				n->setPixel(GetPixel(x*width / (width*scale), y* height /(height*scale)), x, y);
			}
		}

		*this = n;

		delete n;
	}

	static void CreateIlluminationMap(Image* img, Image& out, int scale = 1);
	

	int getWidth() { return width; }
	int getHeight() { return height; }
	int getNumOfChannels() { return channels; }
private:
	unsigned char *pixels;
	int width, height, channels;
};
