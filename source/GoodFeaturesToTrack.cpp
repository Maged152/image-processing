#include "GoodFeaturesToTrack.hpp"
#include "Max.hpp"
#include "common/solve.hpp"
#include "common/StructureTensor.hpp"
#include <cassert>

namespace qlm
{
    template <pixel_t T>
    std::vector<KeyPoint<float>> qlm::GoodFeaturesToTrack(
        const Image<ImageFormat::GRAY, T> &in, 
        const int max_corners, 
        const double quality_level, 
        const double min_distance, 
        const int block_size, 
        const int gradient_size,
        const bool use_harris_detector,
        const double k, 
        const BorderMode<ImageFormat::GRAY, T> &border_mode)
    {
        // validate the input parameters
        assert(min_distance >= 0);
        assert(quality_level > 0);
        assert(quality_level <= 1);
        assert(block_size % 2 == 1);
        assert(gradient_size % 2 == 1);

        const int max_num_corners = max_corners <= 0 ? in.Width() * in.Height() : max_corners;

        std::vector<KeyPoint<float>> out;
        constexpr float neg_inf = std::numeric_limits<float>::lowest();
        const double min_distance_squared = min_distance * min_distance;
        const int valid_distance = block_size / 2 + gradient_size / 2;
        const qlm::Rectangle<int> roi{ qlm::Point<int>{valid_distance, valid_distance}, in.Width() - valid_distance, in.Height() - valid_distance };
        const BorderMode<ImageFormat::GRAY, float> border_mode_f{border_mode.border_type, border_mode.border_pixel.v};

        // stage 1 & 2: compute the gradients and the sum of squared gradients
        // in a block_size x block_size window
        Image<ImageFormat::GRAY, int16_t> sobel_x, sobel_y;
        Image<ImageFormat::GRAY, float> Ixx_sum, Iyy_sum, Ixy_sum;
        StructureTensor(in, gradient_size, block_size, block_size, sobel_x, sobel_y, Ixx_sum, Iyy_sum, Ixy_sum, border_mode, border_mode_f, roi);

        // stage 3: compute the corner response
        Image<ImageFormat::GRAY, float> response = CornerResponse(Ixx_sum, Iyy_sum, Ixy_sum, k, 1.0f, use_harris_detector, roi);
        const Pixel<ImageFormat::GRAY, float> max_response_pixel = qlm::Max(response);

        // stage 4: threshold the response and find corners
        const float threshold = max_response_pixel.v * quality_level;

        for (int y = valid_distance; y < response.Height() - valid_distance; y++)
        {
            for (int x = valid_distance; x < response.Width() - valid_distance; x++)
            {
                const float response_value = response.GetPixel(x, y).v;
                if (response_value < threshold)
                {
                    response.SetPixel(x, y, neg_inf);
                }
            }
        }

        // stage 5: non-maximum suppression
        std::vector<KeyPoint<float>> corners;
        constexpr int num_neighbors = 8;
        static constexpr qlm::Point<int> neighbor_idx[8] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

        for (int y = valid_distance; y < response.Height() - valid_distance; y++)
        {
            for (int x = valid_distance; x < response.Width() - valid_distance; x++)
            {
                const float response_value = response.GetPixel(x, y).v;
                if (response_value == neg_inf)
                {
                    continue;
                }

                bool is_local_maximum = true;
                for (int i = 0; i < num_neighbors; i++)
                {
                    const int neighbor_x = x + neighbor_idx[i].x;
                    const int neighbor_y = y + neighbor_idx[i].y;

                    if (neighbor_x >= valid_distance && neighbor_x < response.Width() - valid_distance && 
                        neighbor_y >= valid_distance && neighbor_y < response.Height() - valid_distance)
                    {
                        const float neighbor_response_value = response.GetPixel(neighbor_x, neighbor_y).v;
                        if (neighbor_response_value > response_value)
                        {
                            is_local_maximum = false;
                            break;
                        }
                    }
                }

                if (is_local_maximum)
                {
                    corners.emplace_back(Point<float>(x, y), response_value, 1.0f); // use scale to track if the corner is removed or not : 1 -> exists, 0 -> removed 
                }
            }
        }

        // stage 6: radius NMS
        // deterministic sort: response desc, then y asc, then x asc
        std::sort(corners.begin(), corners.end(), [](const KeyPoint<float>& a, const KeyPoint<float>& b) {
            if (a.response != b.response) return a.response > b.response;
            if (a.point.y != b.point.y)   return a.point.y < b.point.y;
            return a.point.x < b.point.x;
        });

        const auto distance = [](const KeyPoint<int>& a, const KeyPoint<int>& b) {
            const int dx = a.point.x - b.point.x;
            const int dy = a.point.y - b.point.y;
            return dx * dx + dy * dy;
        };

        for (int i = 0; i < corners.size(); i++)
        {
            if (corners[i].scale)
            {
                // add corner to the output list
                out.push_back(corners[i]);

                // remove corners within the min_distance
                for (size_t j = i + 1; j < corners.size(); j++)
                {
                    if (corners[j].scale)
                    {
                        if (distance(corners[i], corners[j]) < min_distance_squared)
                            corners[j].scale = 0; // Mark as removed
                    }
                }

                if (out.size() == max_num_corners)
                    break;
            }
        }

        return out;
    }

    template std::vector<KeyPoint<float>> 
    qlm::GoodFeaturesToTrack<uint8_t>(const Image<ImageFormat::GRAY, uint8_t>&, 
        const int, const double, 
        const double, const int, 
        const int, const bool, 
        const double, const BorderMode<ImageFormat::GRAY, uint8_t>&);
}
