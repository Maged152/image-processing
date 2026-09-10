#define _USE_MATH_DEFINES

#include "Rotate.hpp"
#include <cmath>
#include <functional>

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Rotate(const Image<frmt, T>& in, const RotateFlag rotate_code)
	{
		int img_width = in.width;
		int img_height = in.height;

		// output image
		Image<frmt, T> out = Image<frmt, T>{};

		using RotateLambda = std::function<void(int, int, const Pixel<frmt, T>&)>;


		// rotate 180
		RotateLambda rotate_180_lambda = [img_width, img_height, &in, &out](int x, int y, const Pixel<frmt, T>& pix)
		{
			out.SetPixel(img_width -1 - x, img_height - 1 - y, pix);
		};

		// rotate 90
		RotateLambda rotate_90_lambda = [img_width, img_height, &in, &out](int x, int y, const Pixel<frmt, T>& pix)
		{
			out.SetPixel(img_height - 1 - y, x, pix);
		};

		// rotate -90
		RotateLambda rotate_270_lambda = [img_width, img_height, &in, &out](int x, int y, const Pixel<frmt, T>& pix)
		{
			out.SetPixel(y, img_width - 1 - x, pix);
		};

		// function type
		RotateLambda rotate_fun;

		if (rotate_code == RotateFlag::ROTATE_180)
		{
			// 180 degrees
			out.Create(img_width, img_height);

			rotate_fun = rotate_180_lambda;
		}
		else
		{
			// 90 or 270 degrees, swap width , height
			out.Create(img_height, img_width);

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
				const auto pix = in.GetPixel(x, y);
				rotate_fun(x, y, pix);
			}
		}
		

		return out;
	}


	template Image<ImageFormat::RGB, uint8_t> Rotate(const Image<ImageFormat::RGB, uint8_t>&, const RotateFlag);
	template Image<ImageFormat::GRAY, uint8_t> Rotate(const Image<ImageFormat::GRAY, uint8_t>&, const RotateFlag);
	template Image<ImageFormat::RGB, int16_t> Rotate(const Image<ImageFormat::RGB, int16_t>&, const RotateFlag);
	template Image<ImageFormat::GRAY, int16_t> Rotate(const Image<ImageFormat::GRAY, int16_t>&, const RotateFlag);
}
