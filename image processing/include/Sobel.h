#pragma once

#include "common/types.h"

namespace qlm
{
	// generate derivative kernel
	Kernel1D GetDerivKernel(size_t filter_size, int order = 1);
	// generate coefficients for Sobel X
	SepKernel GetDerivXKernel(size_t filter_size);
	// generate coefficients for Sobel Y
	SepKernel GetDerivYKernel(size_t filter_size);
	// Sobel X operation
	template<pixel_t in_t, pixel_t out_t = int16_t>
	Image<ImageFormat::GRAY, out_t> SobelX(
		const Image <ImageFormat::GRAY, in_t>& in,
		const size_t kernel_size,
		const BorderMode<ImageFormat::GRAY, in_t>& border_mode = BorderMode<ImageFormat::GRAY, in_t>{}
	);
	// Sobel Y operation
	template<pixel_t in_t, pixel_t out_t = int16_t>
	Image<ImageFormat::GRAY, out_t> SobelY(
		const Image<ImageFormat::GRAY, in_t>& in,
		const size_t kernel_size,
		const BorderMode<ImageFormat::GRAY, in_t>& border_mode = BorderMode<ImageFormat::GRAY, in_t>{}
	);
	// Sobel
	template<pixel_t in_t, pixel_t out_t = int16_t>
	SobelDerivatives<in_t, out_t> Sobel(
		const Image<ImageFormat::GRAY, in_t>& in,
		const size_t kernel_size,
		const BorderMode<ImageFormat::GRAY, in_t>& border_mode = BorderMode<ImageFormat::GRAY, in_t>{}
	);
	// change Bit Depth to sobel from S16 to U8
	Image<ImageFormat::GRAY, uint8_t> ConvertSobelDepth(
		Image < ImageFormat::GRAY, int16_t>& in,
		size_t filter_size);
}