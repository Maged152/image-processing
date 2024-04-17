#include "Transposeh.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Transpose(const Image<frmt, T>& in)
	{
		const size_t img_width = in.Width();
		const size_t img_height = in.Height();
		// output image
		Image<frmt, T> out = Image<frmt, T>{};
		out.create(img_height, img_width);

		// loop over the image
		for (int y = 0; y < img_height; y++)
		{
			for (int x = 0; x < img_width; x++)
			{
				out.SetPixel(y, x, in.GetPixel(x, y));
			}
		}

		return out;
	}


	template Image<ImageFormat::RGB, uint8_t> Transpose(const Image<ImageFormat::RGB, uint8_t>&);
	template Image<ImageFormat::GRAY, uint8_t> Transpose(const Image<ImageFormat::GRAY, uint8_t>&);
	template Image<ImageFormat::RGB, int16_t> Transpose(const Image<ImageFormat::RGB, int16_t>&);
	template Image<ImageFormat::GRAY, int16_t> Transpose(const Image<ImageFormat::GRAY, int16_t>&);
}
