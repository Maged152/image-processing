#include "Transposeh.hpp"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	void Transpose(const Image<frmt, T>& in, Image<frmt, T>& out)
	{
		for (int y = 0; y < in.height; y++)
		{
			for (int x = 0; x < in.width; x++)
			{
				out.SetPixel(y, x, in.GetPixel(x, y));
			}
		}
	}

	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Transpose(const Image<frmt, T>& in)
	{
		// output image
		Image<frmt, T> out{ in.height, in.width };

		Transpose(in, out);

		return out;
	}


	template void Transpose(const Image<ImageFormat::RGB, uint8_t>&, Image<ImageFormat::RGB, uint8_t>&);
	template void Transpose(const Image<ImageFormat::GRAY, uint8_t>&, Image<ImageFormat::GRAY, uint8_t>&);
	template void Transpose(const Image<ImageFormat::RGB, int16_t>&, Image<ImageFormat::RGB, int16_t>&);
	template void Transpose(const Image<ImageFormat::GRAY, int16_t>&, Image<ImageFormat::GRAY, int16_t>&);

	template Image<ImageFormat::RGB, uint8_t> Transpose(const Image<ImageFormat::RGB, uint8_t>&);
	template Image<ImageFormat::GRAY, uint8_t> Transpose(const Image<ImageFormat::GRAY, uint8_t>&);
	template Image<ImageFormat::RGB, int16_t> Transpose(const Image<ImageFormat::RGB, int16_t>&);
	template Image<ImageFormat::GRAY, int16_t> Transpose(const Image<ImageFormat::GRAY, int16_t>&);
}
