#pragma once

#include "common/types.hpp"

namespace qlm
{
	// generate coefficients for Scharr X
	SepKernel GetScharrXKernel();

	// generate coefficients for Scharr Y
	SepKernel GetScharrYKernel();

	// Scharr X operation
	template<pixel_t in_t, pixel_t out_t = int16_t>
	Image<ImageFormat::GRAY, out_t> ScharrX(
		const Image <ImageFormat::GRAY, in_t>& in,
		const BorderMode<ImageFormat::GRAY, in_t>& border_mode = BorderMode<ImageFormat::GRAY, in_t>{}
	);

	// Scharr Y operation
	template<pixel_t in_t, pixel_t out_t = int16_t>
	Image<ImageFormat::GRAY, out_t> ScharrY(
		const Image<ImageFormat::GRAY, in_t>& in,
		const BorderMode<ImageFormat::GRAY, in_t>& border_mode = BorderMode<ImageFormat::GRAY, in_t>{}
	);

	// change Bit Depth to scharr from S16 to U8
	Image<ImageFormat::GRAY, uint8_t> ConvertScharrDepth(
		Image < ImageFormat::GRAY, int16_t>& in);
}