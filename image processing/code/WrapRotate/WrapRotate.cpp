#define _USE_MATH_DEFINES

#include "WrapRotate.h"
#include <cmath>

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> WrapRotate(const Image<frmt, T>& in, float angle, const Point<int>& centre, const float scale, const Pixel<frmt, T>& pix)
	{
		unsigned int img_width = in.Width();
		unsigned int img_height = in.Height();
		// output image
		Image<frmt, T> out = Image<frmt, T>{};
		out.create(img_width, img_height, pix);
		// temp out pixel
		Pixel<frmt, T> out_pix;
		// convert from degree to raduis
		angle *= M_PI / 180.0f;
		float alpha = std::cosf(angle) * scale;
		float beta = std::sinf(angle) * scale;
		// location after rotation
		int x_n{ 0 }, y_n{ 0 };
		// loop over the image
		for (int y = 0; y < img_height; y++)
		{
			for (int x = 0; x < img_width; x++)
			{
				// rotate
				x_n = alpha * (x - centre.x) + beta * (y - centre.y) + centre.x;
				y_n = -beta * (x - centre.x) +  alpha * (y - centre.y) + centre.y;
				// Check if the new coordinates are within the bounds of the output image.
				if (x_n < img_width && x_n >= 0 && y_n < img_height && y_n >= 0)
				{
					out_pix = in.GetPixel(x_n, y_n);
					// store pixel value at (x_n, y_n) in (x, y)
					out.SetPixel(x, y, out_pix);
				}
			}
		}
		return out;
	}


	template Image<ImageFormat::RGB, uint8_t> WrapRotate(const Image<ImageFormat::RGB, uint8_t>&, float, const Point<int>&, const float, const Pixel< ImageFormat::RGB, uint8_t >&);
	template Image<ImageFormat::GRAY, uint8_t> WrapRotate(const Image<ImageFormat::GRAY, uint8_t>&, float angle, const Point<int>&, const float, const Pixel<ImageFormat::GRAY, uint8_t>&);
	template Image<ImageFormat::RGB, int16_t> WrapRotate(const Image<ImageFormat::RGB, int16_t>&, float angle, const Point<int>&, const float, const Pixel<ImageFormat::RGB, int16_t>&);
	template Image<ImageFormat::GRAY, int16_t> WrapRotate(const Image<ImageFormat::GRAY, int16_t>&, float angle, const Point<int>&, const float, const Pixel<ImageFormat::GRAY, int16_t>&);
}
