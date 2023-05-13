#define _USE_MATH_DEFINES

#include "Rotate.h"
#include <cmath>

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Rotate(const Image<frmt, T>& in, const RotateFlag rotate_code)
	{
		unsigned int img_width = in.Width();
		unsigned int img_height = in.Height();
		// output image
		Image<frmt, T> out = Image<frmt, T>{};
		if (rotate_code == RotateFlag::ROTATE_180)
		{
			// 180 degrees
			out.create(img_width, img_height);
		}
		else
		{
			// 90 or 270 degrees, swap width , height
			out.create(img_height, img_width);
		}
		int x_c = (img_width - 1) / 2;
		int y_c = (img_height - 1)/ 2;
		// rotate function
		void (*Rotate_lambda)(int, int, int, int, int&, int&);
		if (rotate_code == RotateFlag::ROTATE_90_CLOCKWISE)
		{
			Rotate_lambda = [](int x, int y, int x_c, int y_c, int& x_n, int& y_n)
			{
				x_n = -y + y_c + x_c;
				y_n = x - x_c + y_c;

			};
		}
		else if (rotate_code == RotateFlag::ROTATE_90_COUNTERCLOCKWISE)
		{
			Rotate_lambda = [](int x, int y, int x_c, int y_c, int& x_n, int& y_n)
			{
				x_n = y - y_c + x_c;
				y_n = -x + x_c + y_c;
			};
		}
		else
		{
			Rotate_lambda = [](int x, int y, int x_c, int y_c, int& x_n, int& y_n)
			{
				x_n = -x + x_c + x_c;
				y_n = -y + y_c + y_c;
			};
		}
		// temp out pixel
		Pixel<frmt, T> out_pix;
		// output location
		int x_n{ 0 }, y_n{ 0 };
		// loop over the image
		for (int y = 0; y < img_height; y++)
		{
			for (int x = 0; x < img_width; x++)
			{
				// rotate
				Rotate_lambda(x, y, x_c, y_c, x_n, y_n);
				// check boundaries
				if (x_n < out.Width() && x_n >= 0 && y_n < out.Height() && y_n >= 0)
				{
					out_pix = in.GetPixel(x, y);
					// store pixel value at (x, y) in (x_n, y_n)
					out.SetPixel(x_n, y_n, out_pix);
				}
			}
		}
		return out;
	}


	template Image<ImageFormat::RGB, uint8_t> Rotate(const Image<ImageFormat::RGB, uint8_t>&, const RotateFlag);
	template Image<ImageFormat::GRAY, uint8_t> Rotate(const Image<ImageFormat::GRAY, uint8_t>&, const RotateFlag);
	template Image<ImageFormat::RGB, int16_t> Rotate(const Image<ImageFormat::RGB, int16_t>&, const RotateFlag);
	template Image<ImageFormat::GRAY, int16_t> Rotate(const Image<ImageFormat::GRAY, int16_t>&, const RotateFlag);
}
