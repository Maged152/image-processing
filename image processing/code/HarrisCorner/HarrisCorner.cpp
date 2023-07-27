#include "HarrisCorner/HarrisCorner.h"
#include "ColorConvert/ColorConvert.h"
#include "Gaussian/Gaussian.h"
#include "Sobel/Sobel.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	std::vector<KeyPoint<int>> HarrisCorner(const Image<frmt, T>& in, const unsigned int block_size, const unsigned int filter_size, float k, float threshold, const Border border_type, const int border_value)
	{
		const unsigned int width = in.Width();
		const unsigned int height = in.Height();

		Image<ImageFormat::GRAY, uint8_t> gray {width, height};
		
		if constexpr (frmt != ImageFormat::GRAY)
		{
			// convert to gray
			gray = ColorConvert<frmt, uint8_t, ImageFormat::GRAY, uint8_t>(in);
		}
		else
		{
			gray = in;
		}
		// call gaussian kernerl 
		int constexpr sigma = 1;
		Image<ImageFormat::GRAY, uint8_t> gaussian = Gaussian(gray, filter_size, sigma, sigma, border_type, border_value);
		// call sobel x and y
		Image<ImageFormat::GRAY, int16_t> sobel_x = SobelX<uint8_t, int16_t>(gaussian, filter_size, border_type, border_value);
		Image<ImageFormat::GRAY, int16_t> sobel_y = SobelY<uint8_t, int16_t>(gaussian, filter_size, border_type, border_value);
		// compute Ixx, Iyy, Ixy
		Image<ImageFormat::GRAY, int16_t> Ixx{ width , height};
		Image<ImageFormat::GRAY, int16_t> Iyy{ width , height };
		Image<ImageFormat::GRAY, int16_t> Ixy{ width , height };

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				auto Ix = sobel_x.GetPixel(x, y);
				auto Iy = sobel_y.GetPixel(x, y);

				Ixx.SetPixel(x, y, Ix * Ix);
				Ixy.SetPixel(x, y, Iy * Ix);
				Iyy.SetPixel(x, y, Iy * Iy);
			}
		}
		// use gaussian filter to sum on block size
		Image<ImageFormat::GRAY, int16_t> Ixx_sum = Gaussian(Ixx, block_size, sigma, sigma, border_type, border_value);
		Image<ImageFormat::GRAY, int16_t> Iyy_sum = Gaussian(Iyy, block_size, sigma, sigma, border_type, border_value);
		Image<ImageFormat::GRAY, int16_t> Ixy_sum = Gaussian(Ixy, block_size, sigma, sigma, border_type, border_value);
		
		std::vector<KeyPoint<int>> key_points;
		// compute R
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				float Ixx = Ixx_sum.GetPixel(x, y).v;
				float Iyy = Iyy_sum.GetPixel(x, y).v;
				float Ixy = Ixy_sum.GetPixel(x, y).v;

				float det = Ixx * Iyy - Ixy * Ixy;
				float tr = Ixx + Iyy;

				float response = det - k * tr * tr;
				// check if it is a corner
				if (response > threshold)
				{
					key_points.emplace_back(KeyPoint<int>{{ x, y}, response });
				}
				// non-max suppression
			}
		}
		return key_points;
	}

	template std::vector<KeyPoint<int>>
	HarrisCorner<ImageFormat::GRAY, uint8_t>(const Image<ImageFormat::GRAY, uint8_t>&, const unsigned int, const unsigned int, float, float, const Border, const int);
	
	template std::vector<KeyPoint<int>>
	HarrisCorner<ImageFormat::RGB, uint8_t>(const Image<ImageFormat::RGB, uint8_t>&, const unsigned int, const unsigned int, float, float, const Border, const int);

}
