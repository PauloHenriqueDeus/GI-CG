#include "Image.h"
#include "Filter.h"

void Image::CreateIlluminationMap(Image * img, Image & out, int scale)
{ //scale >= 1
	scale = (scale < 1) ? 1 : scale;


	Image ilumination = Image(img->getWidth(), img->getHeight());
	Filter::ApplyGrayscale(img, ilumination, Vector3(0.27f, 0.67f, 0.06f), false);

	if (scale > 1) {
		out = Image(img->getWidth() / scale, img->getHeight() / scale);

		for (int x = 0; x < out.width; x++) {
			for (int y = 0; y < out.height; y++)
			{
				//n->setPixel(GetPixel(x*width / (width*scale), y* height / (height*scale)), x, y);

				int h = scale;
				int w = ((float)img->getWidth() / img->getHeight()) * scale;

				Vector3 avg;

				for (int i = 0; i < w; i++) {
					for (int j = 0; j < h; j++)
					{
						avg += ilumination.GetPixel((x*((float)img->getWidth() / out.width)) + i, (y*((float)img->getHeight() / out.height)) + j).ToVector3();
					}
				}

				Color c = Color(avg / (h*w), 1.0f);
				out.setPixel(c, x, y);
			}
		}
	}
	else {
		out = ilumination;
	}
}
