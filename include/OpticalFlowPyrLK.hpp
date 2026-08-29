#pragma once

#include "common/types.hpp"

namespace qlm
{
	template<pixel_t T>
    std::vector<KeyPoint<float>> OpticalFlowPyrLK(
        const Image<ImageFormat::GRAY, T>& prev_img,
        const Image<ImageFormat::GRAY, T>& next_img,
        const std::vector<KeyPoint<float>>& prev_pts,
        const std::vector<KeyPoint<float>>& initial_guess,
        const Size& win_size = Size(21, 21),
        const int max_level = 3,
        const TermCriteria& criteria = TermCriteria(30, 0.01),
        const double min_eig_threshold = 1e-4
    );
}