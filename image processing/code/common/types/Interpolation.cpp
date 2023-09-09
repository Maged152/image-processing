#include "common/types/Interpolation.h"

namespace qlm
{
	// Nearest neighbor interpolation
	template<ImageFormat frmt, pixel_t T>
	Pixel<frmt, T> NearestNeighborInterpolation(const Image<frmt, T>& src, float x, float y, const BorderMode<frmt, T>& border_mode)
	{
		return Pixel<frmt, T>();
	}

	// Bilinear interpolation
	template<ImageFormat frmt, pixel_t T>
	Pixel<frmt, T> BilinearInterpolation(const Image<frmt, T>& src, float x, float y, const BorderMode<frmt, T>& border_mode)
	{
		return Pixel<frmt, T>();
	}

	//Quadratic interpolation
	template<ImageFormat frmt, pixel_t T>
	Pixel<frmt, T> BicubicInterpolation(const Image<frmt, T>& src, float x, float y, const BorderMode<frmt, T>& border_mode)
	{
		return Pixel<frmt, T>();
	}

	// interpolation
	template<ImageFormat frmt, pixel_t T>
	Pixel<frmt, T> Interpolation(const Image<frmt, T>& src, float x, float y, const InterpolationFlag flag, const BorderMode<frmt, T>& border_mode)
	{
		if (method == qlm::InterpolationFlag::NEAREST_NEIGHBOR)
		{
			return NearestNeighborInterpolation(src, x, y, border_mode);
		}
		else if (method == qlm::InterpolationFlag::BILINEAR)
		{
			return BilinearInterpolation(src, x, y, border_mode);
		}
		else
		{
			return BicubicInterpolation(src, x, y, border_mode);
		}
	}
}