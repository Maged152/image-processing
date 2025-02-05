#include "Translate.hpp"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Translate(const Image<frmt, T>& in, const Point<int>& displacement, const Pixel<frmt, T>& pix)
	{
		int img_width = in.width;
		int img_height = in.height;
		// output image
		Image<frmt, T> out = Image<frmt, T>{};
		out.create(img_width, img_height, pix);
		// temp out pixel
		Pixel<frmt, T> out_pix;
		// location after rotation
		int x_n{ 0 }, y_n{ 0 };
		// loop over the image
		for (int y = 0; y < img_height; y++)
		{
			for (int x = 0; x < img_width; x++)
			{
				// rotate
				x_n = x + displacement.x;
				y_n = y + displacement.y;
				// Check if the new coordinates are within the bounds of the output image.
				if (x_n < img_width && x_n >= 0 && y_n < img_height && y_n >= 0)
				{
					out_pix = in.GetPixel(x, y);
					// store pixel value at (x_n, y_n) in (x, y)
					out.SetPixel(x_n, y_n, out_pix);
				}
			}
		}
		return out;
	}


	template Image<ImageFormat::RGB, uint8_t>  Translate(const Image<ImageFormat::RGB, uint8_t>&, const Point<int>&, const Pixel< ImageFormat::RGB, uint8_t >&);
	template Image<ImageFormat::GRAY, uint8_t> Translate(const Image<ImageFormat::GRAY, uint8_t>&, const Point<int>&, const Pixel<ImageFormat::GRAY, uint8_t>&);
	template Image<ImageFormat::RGB, int16_t>  Translate(const Image<ImageFormat::RGB, int16_t>&, const Point<int>&, const Pixel<ImageFormat::RGB, int16_t>&);
	template Image<ImageFormat::GRAY, int16_t> Translate(const Image<ImageFormat::GRAY, int16_t>&, const Point<int>&, const Pixel<ImageFormat::GRAY, int16_t>&);
}
