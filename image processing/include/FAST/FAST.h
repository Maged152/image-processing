#pragma once

#include "common/types.h"
#include <vector>


namespace qlm
{
	template<pixel_t T>
	std::vector<KeyPoint<int>> FAST(
		const Image<ImageFormat::GRAY, T>& in,
		const int arc_length = 9,
		const int threshold = 10,
		const bool non_max_suppression = true);
}