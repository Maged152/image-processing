#include "common/types/Interpolation.hpp"
#include <cmath>

namespace qlm
{
	// Nearest neighbor interpolation
	template<ImageFormat frmt, pixel_t T>
	Pixel<frmt, T> NearestNeighborInterpolation(const Image<frmt, T>& src, float x, float y, const BorderMode<frmt, T>& border_mode)
	{
		int x_loc = static_cast<int>(std::roundf(x));
		int y_loc = static_cast<int>(std::roundf(y));

		return src.GetPixel(x_loc, y_loc, border_mode);
	}

	template  Pixel<ImageFormat::GRAY, uint8_t> NearestNeighborInterpolation<ImageFormat::GRAY, uint8_t>(const Image<ImageFormat::GRAY, uint8_t>&, float, float, const BorderMode<ImageFormat::GRAY, uint8_t>&);
	template  Pixel<ImageFormat::RGB, uint8_t> NearestNeighborInterpolation<ImageFormat::RGB, uint8_t>(const Image<ImageFormat::RGB, uint8_t>&, float, float, const BorderMode<ImageFormat::RGB, uint8_t>&);


	// Bilinear interpolation
	template<ImageFormat frmt, pixel_t T>
	Pixel<frmt, T> BilinearInterpolation(const Image<frmt, T>& src, float x, float y, const BorderMode<frmt, T>& border_mode)
	{
		// get the four points
		int x1 = std::floor(x);
		int y1 = std::floor(y);
		int x2 = std::ceil(x);
		int y2 = std::ceil(y);
		// get the four neighbors
		qlm::Pixel<frmt, T> tl = src.GetPixel(x1, y1, border_mode);
		qlm::Pixel<frmt, T> tr = src.GetPixel(x2, y1, border_mode);
		qlm::Pixel<frmt, T> bl = src.GetPixel(x1, y2, border_mode);
		qlm::Pixel<frmt, T> br = src.GetPixel(x2, y2, border_mode);
		// offsets
		float x_offset = x - x1;
		float y_offset = y - y1;
		// interpolate along x-axis
		qlm::Pixel<frmt, T> top = tl * (1.0f - x_offset) + tr * x_offset;
		qlm::Pixel<frmt, T> bottom = bl * (1.0f - x_offset) + br * x_offset;
		// interpolate along y-axis
		qlm::Pixel<frmt, T> out_pixel = top * (1.0f - y_offset) + bottom * y_offset;
		
		return out_pixel;
	}

	template  Pixel<ImageFormat::GRAY, uint8_t> BilinearInterpolation<ImageFormat::GRAY, uint8_t>(const Image<ImageFormat::GRAY, uint8_t>&, float, float, const BorderMode<ImageFormat::GRAY, uint8_t>&);
	template  Pixel<ImageFormat::RGB, uint8_t> BilinearInterpolation<ImageFormat::RGB, uint8_t>(const Image<ImageFormat::RGB, uint8_t>&, float, float, const BorderMode<ImageFormat::RGB, uint8_t>&);

	// Quadratic interpolation
	template<ImageFormat frmt, pixel_t T>
	Pixel<frmt, T> BicubicInterpolation(const Image<frmt, T>& src, float x, float y, const BorderMode<frmt, T>& border_mode)
	{
		size_t x1 = std::floor(x);
		size_t y1 = std::floor(y);
		// offsets
		float xoff = x - x1;
		float yoff = y - y1;
		float xxoff{ xoff * xoff }, xxxoff{ xoff * xoff * xoff };
		float yyoff{ yoff * yoff }, yyyoff{ yoff * yoff * yoff };

		// 16 point to sample
		qlm::Pixel<frmt, float> bicubic_points[4][4]{};
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				int px = x1 - 1 + i;
				int py = y1 - 1 + j;
				bicubic_points[i][j] = src.GetPixel(px, py, border_mode);
			}
		}
		// x offsets
		float Q1_x = 0.5f * (-xxxoff + 2.0f * xxoff - xoff);
		float Q2_x = 0.5f * (3.0f * xxxoff - 5.0f * xxoff + 2.0f);
		float Q3_x = 0.5f * (-3.0f * xxxoff + 4.0f * xxoff + xoff);
		float Q4_x = 0.5f * (xxxoff - xxoff);
		// y offsets
		float Q1_y = 0.5f * (-yyyoff + 2.0f * yyoff - yoff);
		float Q2_y = 0.5f * (3.0f * yyyoff - 5.0f * yyoff + 2.0f);
		float Q3_y = 0.5f * (-3.0f * yyyoff + 4.0f * yyoff + yoff);
		float Q4_y = 0.5f * (yyyoff - yyoff);

		qlm::Pixel<frmt, float> bicubic_points_y[4]{};
		for (int i = 0; i < 4; i++)
		{
			bicubic_points_y[i] = bicubic_points[i][0] * Q1_x + bicubic_points[i][1] * Q2_x + bicubic_points[i][2] * Q3_x + bicubic_points[i][3] * Q4_x;
		}

		qlm::Pixel<frmt, T> out_pixel;
		out_pixel = bicubic_points_y[0] * Q1_y + bicubic_points_y[1] * Q2_y + bicubic_points_y[2] * Q3_y + bicubic_points_y[3] * Q4_y;

		return out_pixel;
	}

	template  Pixel<ImageFormat::GRAY, uint8_t> BicubicInterpolation<ImageFormat::GRAY, uint8_t>(const Image<ImageFormat::GRAY, uint8_t>&, float, float, const BorderMode<ImageFormat::GRAY, uint8_t>&);
	template  Pixel<ImageFormat::RGB, uint8_t> BicubicInterpolation<ImageFormat::RGB, uint8_t>(const Image<ImageFormat::RGB, uint8_t>&, float, float, const BorderMode<ImageFormat::RGB, uint8_t>&);

	// interpolation
	template<ImageFormat frmt, pixel_t T>
	Pixel<frmt, T> Interpolation(const Image<frmt, T>& src, float x, float y, const InterpolationFlag flag, const BorderMode<frmt, T>& border_mode)
	{
		if (flag == qlm::InterpolationFlag::NEAREST_NEIGHBOR)
		{
			return NearestNeighborInterpolation(src, x, y, border_mode);
		}
		else if (flag == qlm::InterpolationFlag::BILINEAR)
		{
			return BilinearInterpolation(src, x, y, border_mode);
		}
		else
		{
			return BicubicInterpolation(src, x, y, border_mode);
		}
	}

	template  Pixel<ImageFormat::GRAY, uint8_t> Interpolation<ImageFormat::GRAY, uint8_t>(const Image<ImageFormat::GRAY, uint8_t>&, float, float, const InterpolationFlag, const BorderMode<ImageFormat::GRAY, uint8_t>&);
	template  Pixel<ImageFormat::RGB, uint8_t> Interpolation<ImageFormat::RGB, uint8_t>(const Image<ImageFormat::RGB, uint8_t>&, float, float, const InterpolationFlag, const BorderMode<ImageFormat::RGB, uint8_t>&);

}