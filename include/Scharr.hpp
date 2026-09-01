#pragma once

#include "common/types.hpp"

namespace qlm
{
	template<pixel_t mag = uint8_t, pixel_t drv = int16_t>
	struct ScharrDerivatives
	{
	public:
		Image<ImageFormat::GRAY, drv> scharr_x;
		Image<ImageFormat::GRAY, drv> scharr_y;
		Image<ImageFormat::GRAY, mag> magnitude;
		Image<ImageFormat::GRAY, float> angle;
	public:
		ScharrDerivatives() = default;
		ScharrDerivatives(int width, int height) : scharr_x(width, height), scharr_y(width, height), magnitude(width, height), angle(width, height)
		{}

	};

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

	// Scharr
	template<pixel_t in_t, pixel_t out_t = int16_t>
	ScharrDerivatives<in_t, out_t> Scharr(
		const Image<ImageFormat::GRAY, in_t>& in,
		const BorderMode<ImageFormat::GRAY, in_t>& border_mode = BorderMode<ImageFormat::GRAY, in_t>{}
	);

	// change Bit Depth to scharr from S16 to U8
	Image<ImageFormat::GRAY, uint8_t> ConvertScharrDepth(
		Image < ImageFormat::GRAY, int16_t>& in);
}