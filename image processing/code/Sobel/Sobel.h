#pragma once

#include "types.h"

namespace qlm
{
	// generate derivative kernel
	Kernel1D GetDerivKernel(unsigned int n, int order = 1);
	// generate coefficients for Sobel X
	SepKernel GetDerivXKernel(unsigned int n);
	// generate coefficients for Sobel Y
	SepKernel GetDerivYKernel(unsigned int n);
	// Sobel X operation
	template<pixel_t in_t, pixel_t out_t = int16_t>
	Image<ImageFormat::GRAY, out_t> SobelX(
		const Image <ImageFormat::GRAY, in_t>& in,
		const unsigned int kernel_size,
			  Border border_type = Border::BORDER_CONSTANT,
			  int border_value = 0
	);
	// Sobel Y operation
	template<pixel_t in_t, pixel_t out_t = int16_t>
	Image<ImageFormat::GRAY, out_t> SobelY(
		const Image<ImageFormat::GRAY, in_t>& in,
		const unsigned int kernel_size,
			  Border border_type = Border::BORDER_CONSTANT,
			  int border_value = 0
	);
	// Sobel
	template<pixel_t in_t, pixel_t out_t = int16_t>
	SobelDerivatives<in_t, out_t> Sobel(
		const Image<ImageFormat::GRAY, in_t>& in,
		const unsigned int kernel_size,
		      Border border_type = Border::BORDER_CONSTANT,
		      int border_value = 0
	);
	// change Bit Depth to sobel from S16 to U8
	Image<ImageFormat::GRAY, uint8_t> ConvertSobelDepth(
		Image < ImageFormat::GRAY, int16_t>& in,
		unsigned int filter_size);
}