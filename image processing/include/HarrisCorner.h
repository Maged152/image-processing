#pragma once
#include "common/types.h"
#include <vector>

namespace qlm
{
	template<ImageFormat frmt, pixel_t T = uint8_t>
	std::vector<KeyPoint<int>> HarrisCorner(
		const Image<frmt, T>& in,
		const size_t block_size,
		const size_t filter_size,
		float k,
		float threshold,
		const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{}
	);
}