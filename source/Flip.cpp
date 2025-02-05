#define _USE_MATH_DEFINES

#include "Flip.hpp"
#include <cmath>
#include <functional>

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Flip(const Image<frmt, T>& in, const FlipFlag Flip_code)
	{
		const int img_width = in.width;
		const int img_height = in.height;
		// output image
		Image<frmt, T> out = Image<frmt, T>{};
		out.create(img_width, img_height);
		// output location
		int x_n{ 0 }, y_n{ 0 };
		// Flip vertical 
		std::function<void(int, int)> Flip_vertical_lambda = [img_width, img_height, &in, &out](int x, int y)
		{
			out.SetPixel(x, y, in.GetPixel(img_width - 1 - x, y));
			out.SetPixel(img_width - 1 - x, y, in.GetPixel(x, y));
		};
		// Flip horizontal 
		std::function<void(int, int)> Flip_horizontal_lambda = [img_width, img_height, &in, &out](int x, int y)
		{
			out.SetPixel(x, y, in.GetPixel(x, img_height - 1 - y));
			out.SetPixel(x, img_height - 1 - y, in.GetPixel(x, y));
		};
		// Flip both
		std::function<void(int, int)> Flip_both_lambda = [img_width, img_height, &in, &out](int x, int y)
		{
			out.SetPixel(x, y, in.GetPixel(img_width - 1 - x, img_height - 1 - y));
			out.SetPixel(img_width - 1 - x, img_height - 1 - y, in.GetPixel(x, y));

			out.SetPixel(img_width - 1 - x, y, in.GetPixel(x, img_height - 1 - y));
			out.SetPixel(x, img_height - 1 - y, in.GetPixel(img_width - 1 - x, y));
		};
		// function type
		std::function<void(int, int)> Flip_fun;
		int width{ img_width }, height{ img_height };

		if (Flip_code == FlipFlag::FLIP_VERTICAL_AXIS)
		{
			width /= 2;
			Flip_fun = Flip_vertical_lambda;
		}
		else if (Flip_code == FlipFlag::FLIP_HORIZONTAL_AXIS)
		{
			height /= 2;
			Flip_fun = Flip_horizontal_lambda;
		}
		else
		{
			height /= 2;
			width /= 2;
			Flip_fun = Flip_both_lambda;
		}

		// loop over the image
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				Flip_fun(x, y);
			}
		}

		return out;
	}


	template Image<ImageFormat::RGB, uint8_t> Flip(const Image<ImageFormat::RGB, uint8_t>&, const FlipFlag);
	template Image<ImageFormat::GRAY, uint8_t> Flip(const Image<ImageFormat::GRAY, uint8_t>&, const FlipFlag);
	template Image<ImageFormat::RGB, int16_t> Flip(const Image<ImageFormat::RGB, int16_t>&, const FlipFlag);
	template Image<ImageFormat::GRAY, int16_t> Flip(const Image<ImageFormat::GRAY, int16_t>&, const FlipFlag);
}
