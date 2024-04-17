#define _USE_MATH_DEFINES

#include "Rotate.h"
#include <cmath>
#include <functional>

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Rotate(const Image<frmt, T>& in, const RotateFlag rotate_code)
	{
		size_t img_width = in.Width();
		size_t img_height = in.Height();
		// output image
		Image<frmt, T> out = Image<frmt, T>{};
		// temp out pixel
		Pixel<frmt, T> out_pix;
		// output location
		int x_n{ 0 }, y_n{ 0 };
		// rotate 180
		std::function<void(int, int)> rotate_180_lambda = [img_width, img_height, &in, &out, &out_pix](int x, int y)
		{
			out_pix = in.GetPixel(img_width -1 - x, img_height - 1 - y);
			out.SetPixel(x, y, out_pix);
		};
		// rotate 90
		std::function<void(int, int)> rotate_90_lambda = [img_width, img_height, &in, &out, &out_pix](int x, int y)
		{
			out_pix = in.GetPixel(x, y);
			out.SetPixel(img_width - 1 - y, x, out_pix);
		};
		// rotate -90
		std::function<void(int, int)> rotate_270_lambda = [img_width, img_height, &in, &out, &out_pix](int x, int y)
		{
			out_pix = in.GetPixel(x, y);
			out.SetPixel(y, img_height - 1 - x, out_pix);
		};
		// function type
		std::function<void(int, int)> rotate_fun;

		if (rotate_code == RotateFlag::ROTATE_180)
		{
			// 180 degrees
			out.create(img_width, img_height);

			rotate_fun = rotate_180_lambda;
		}
		else
		{
			// 90 or 270 degrees, swap width , height
			out.create(img_height, img_width);

			if (rotate_code == RotateFlag::ROTATE_90_CLOCKWISE)
			{
				rotate_fun = rotate_90_lambda;
			}
			else
			{
				rotate_fun = rotate_270_lambda;
			}
		}
		
		// loop over the image
		for (int y = 0; y < img_height; y++)
		{
			for (int x = 0; x < img_width; x++)
			{
				rotate_fun(x, y);
			}
		}
		

		return out;
	}


	template Image<ImageFormat::RGB, uint8_t> Rotate(const Image<ImageFormat::RGB, uint8_t>&, const RotateFlag);
	template Image<ImageFormat::GRAY, uint8_t> Rotate(const Image<ImageFormat::GRAY, uint8_t>&, const RotateFlag);
	template Image<ImageFormat::RGB, int16_t> Rotate(const Image<ImageFormat::RGB, int16_t>&, const RotateFlag);
	template Image<ImageFormat::GRAY, int16_t> Rotate(const Image<ImageFormat::GRAY, int16_t>&, const RotateFlag);
}
