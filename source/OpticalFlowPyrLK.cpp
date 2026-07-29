#include "OpticalFlowPyrLK.hpp"

namespace qlm
{
    template <pixel_t T>
    std::vector<KeyPoint<float>> OpticalFlowPyrLK(
        const Image<ImageFormat::GRAY, T> &prev_img, const Image<ImageFormat::GRAY, T> &next_img, 
        const std::vector<KeyPoint<int>> &prev_pts, const std::vector<KeyPoint<int>> &initial_guess, 
        const Size &win_size, const TermCriteria &criteria, const int max_level, const double min_eig_threshold)
    {
        return std::vector<KeyPoint<float>>();
    }
} 
