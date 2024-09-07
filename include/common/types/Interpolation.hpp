#pragma once
#include "common/image.hpp"

namespace qlm
{
	enum class InterpolationFlag
	{
		NEAREST_NEIGHBOR,
		BILINEAR,
		BICUBIC
	};

	// interpolation functions
	template<ImageFormat frmt, pixel_t T>
	Pixel<frmt, T> NearestNeighborInterpolation(const Image<frmt, T>& src, float x, float y, const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{});

	template<ImageFormat frmt, pixel_t T>
	Pixel<frmt, T> BilinearInterpolation(const Image<frmt, T>& src, float x, float y, const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{});

	template<ImageFormat frmt, pixel_t T>
	Pixel<frmt, T> BicubicInterpolation(const Image<frmt, T>& src, float x, float y, const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{});

	template<ImageFormat frmt, pixel_t T>
	Pixel<frmt, T> Interpolation(const Image<frmt, T>& src, float x, float y, const InterpolationFlag flag = InterpolationFlag::BILINEAR, const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{});
}