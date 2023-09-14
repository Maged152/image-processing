#include "common/types/Interpolation.h"
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

	// Bilinear interpolation
	template<ImageFormat frmt, pixel_t T>
	Pixel<frmt, T> BilinearInterpolation(const Image<frmt, T>& src, float x, float y, const BorderMode<frmt, T>& border_mode)
	{
		// get the four points
		int x1 = std::floorf(x);
		int y1 = std::floorf(y);
		int x2 = std::ceilf(x);
		int y2 = std::ceilf(y);
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

	// Quadratic interpolation
	template<ImageFormat frmt, pixel_t T>
	Pixel<frmt, T> BicubicInterpolation(const Image<frmt, T>& src, float x, float y, const BorderMode<frmt, T>& border_mode)
	{
		return Pixel<frmt, T>();
	}

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
}