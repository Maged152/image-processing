#pragma once

#include "common/types.hpp"

namespace qlm
{
	template<pixel_t T>
	Image<ImageFormat::GRAY, T> Canny(
		const Image<ImageFormat::GRAY, T>& in,
		const int threshold_low, const int threshold_high,
		const int filter_size = 3,
		const bool l2_gradient = false ,
		const BorderMode<ImageFormat::GRAY, T>& border_mode = BorderMode<ImageFormat::GRAY, T>{}
	);
}