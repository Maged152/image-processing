#pragma once
#include "common/types.hpp"
#include <vector>

namespace qlm
{
	template<pixel_t T = uint8_t>
	std::vector<KeyPoint<int>> GoodFeaturesToTrack(
		const Image<ImageFormat::GRAY, T>& in,
		const int max_corners,
		const double quality_level, 
		const double min_distance,
		const int block_size = 3,
		const int  gradient_size = 3,
		const bool use_harris_detector = false,
		const double k = 0.04,
		const BorderMode<ImageFormat::GRAY, T>& border_mode = BorderMode<ImageFormat::GRAY, T>{}
	);
}