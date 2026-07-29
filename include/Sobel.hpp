#pragma once

#include "common/types.hpp"

namespace qlm
{
	template<pixel_t mag = uint8_t, pixel_t drv = int16_t>
	struct SobelDerivatives
	{
	public:
		Image<ImageFormat::GRAY, drv> sobel_x;
		Image<ImageFormat::GRAY, drv> sobel_y;
		Image<ImageFormat::GRAY, mag> magnitude;
		Image<ImageFormat::GRAY, float> angle;
	public:
		SobelDerivatives() = default;
		SobelDerivatives(int width, int height) : sobel_x(width, height), sobel_y(width, height), magnitude(width, height), angle(width, height)
		{}
		
	};

	// generate derivative kernel
	Kernel1D GetDerivKernel(int filter_size, int order = 1);

	// generate coefficients for Sobel X
	SepKernel GetDerivXKernel(int filter_size);

	// generate coefficients for Sobel Y
	SepKernel GetDerivYKernel(int filter_size);

	// Sobel X operation
	template<pixel_t in_t, pixel_t out_t = int16_t>
	Image<ImageFormat::GRAY, out_t> SobelX(
		const Image <ImageFormat::GRAY, in_t>& in,
		const int kernel_size,
		const BorderMode<ImageFormat::GRAY, in_t>& border_mode = BorderMode<ImageFormat::GRAY, in_t>{}
	);

	// Sobel Y operation
	template<pixel_t in_t, pixel_t out_t = int16_t>
	Image<ImageFormat::GRAY, out_t> SobelY(
		const Image<ImageFormat::GRAY, in_t>& in,
		const int kernel_size,
		const BorderMode<ImageFormat::GRAY, in_t>& border_mode = BorderMode<ImageFormat::GRAY, in_t>{}
	);

	// Sobel
	template<pixel_t in_t, pixel_t out_t = int16_t>
	SobelDerivatives<in_t, out_t> Sobel(
		const Image<ImageFormat::GRAY, in_t>& in,
		const int kernel_size,
		const BorderMode<ImageFormat::GRAY, in_t>& border_mode = BorderMode<ImageFormat::GRAY, in_t>{}
	);
	
	// change Bit Depth to sobel from S16 to U8
	Image<ImageFormat::GRAY, uint8_t> ConvertSobelDepth(
		Image < ImageFormat::GRAY, int16_t>& in,
		int filter_size);
}