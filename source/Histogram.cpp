#define _USE_MATH_DEFINES

#include "Histogram.hpp"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Histogram_t<frmt, T> Histogram(const Image<frmt, T>& in)
	{
        Histogram_t<frmt, T> out;
		
        for (int y = 0; y < in.height; y++)
		{
			for (int x = 0; x < in.width; x++)
			{
                const auto pix = in.GetPixel(x, y);
                if constexpr (frmt == ImageFormat::GRAY)
                {
                    out.hist[0][pix.v]++;
                }
                else if constexpr (frmt == ImageFormat::RGB)
                {
                    out.hist[0][pix.r]++;
                    out.hist[1][pix.g]++;
                    out.hist[2][pix.b]++;
                }
			}
		}

        return out;
	}


	template Histogram_t<ImageFormat::RGB, uint8_t> Histogram(const Image<ImageFormat::RGB, uint8_t>&);
	template Histogram_t<ImageFormat::GRAY, uint8_t> Histogram(const Image<ImageFormat::GRAY, uint8_t>&);
}
