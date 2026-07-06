#include "GoodFeaturesToTrack.hpp"
#include "Sobel.hpp"
#include "BoxFilter.hpp"
#include "Multiply.hpp"
#include <cassert>

namespace qlm
{
    template <pixel_t T>
    std::vector<KeyPoint<int>> qlm::GoodFeaturesToTrack(
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

        const int max_num_corners = max_corners <= 0 ? in.width * in.height : max_corners;

        std::vector<KeyPoint<int>> out;
        constexpr float neg_inf = std::numeric_limits<float>::lowest();
        const double min_distance_squared = min_distance * min_distance;
        const int valid_distance = block_size / 2 + gradient_size / 2;

        // stage 1: compute the gradient of the image
        const Image<ImageFormat::GRAY, int16_t> sobel_x = qlm::SobelX(in, gradient_size, border_mode);
        const Image<ImageFormat::GRAY, int16_t> sobel_y = qlm::SobelY(in, gradient_size, border_mode);

        // stage 2: compute sum of squared gradients in a block_size x block_size window
        const Image<ImageFormat::GRAY, float> Ixx = qlm::Multiply<ImageFormat::GRAY, int16_t, int16_t, float>(sobel_x, sobel_x, 1.0f, OverFlowFlag::WRAP);
        const Image<ImageFormat::GRAY, float> Iyy = qlm::Multiply<ImageFormat::GRAY, int16_t, int16_t, float>(sobel_y, sobel_y, 1.0f, OverFlowFlag::WRAP);
        const Image<ImageFormat::GRAY, float> Ixy = qlm::Multiply<ImageFormat::GRAY, int16_t, int16_t, float>(sobel_x, sobel_y, 1.0f, OverFlowFlag::WRAP);

        Image<ImageFormat::GRAY, float> Ixx_sum = BoxFilter<ImageFormat::GRAY, float, float>(Ixx, block_size, block_size, false, border_mode);
		Image<ImageFormat::GRAY, float> Iyy_sum = BoxFilter<ImageFormat::GRAY, float, float>(Iyy, block_size, block_size, false, border_mode);
		Image<ImageFormat::GRAY, float> Ixy_sum = BoxFilter<ImageFormat::GRAY, float, float>(Ixy, block_size, block_size, false, border_mode);

        // stage 3: compute the corner response
        Image<ImageFormat::GRAY, float> response (in.width, in.height);
        Pixel<ImageFormat::GRAY, float> response_pixel;
        float max_response_pixel = neg_inf;

        for (int y = valid_distance; y < in.height - valid_distance; y++)
        {
            for (int x = valid_distance; x < in.width - valid_distance; x++)
            {
                const float ixx = Ixx_sum.GetPixel(x, y).v;
                const float ixy = Ixy_sum.GetPixel(x, y).v;
                const float iyy = Iyy_sum.GetPixel(x, y).v;
                
                if (use_harris_detector)
                {
                    const float det = ixx * iyy - ixy * ixy;
                    const float trace = ixx + iyy;
                    response_pixel.v = det - k * trace * trace;
                }
                else
                {
                    float d = (ixx - iyy) * (ixx - iyy) + 4.0f * ixy * ixy;
                    d = std::max(d, 0.0f); // Ensure non-negative value for sqrt
                    response_pixel.v = (ixx + iyy - std::sqrt(d)) * 0.5f;
                }

                // Find maximum response value.
                if (response_pixel.v > max_response_pixel)
                {
                    max_response_pixel = response_pixel.v;
                }

                response.SetPixel(x, y, response_pixel);
            }
        }

        // stage 4: threshold the response and find corners
        const float threshold = max_response_pixel * quality_level;

        for (int y = valid_distance; y < response.height - valid_distance; y++)
        {
            for (int x = valid_distance; x < response.width - valid_distance; x++)
            {
                const float response_value = response.GetPixel(x, y).v;
                if (response_value < threshold)
                {
                    response.SetPixel(x, y, neg_inf);
                }
            }
        }

        // stage 5: non-maximum suppression
        std::vector<KeyPoint<int>> corners;
        constexpr int num_neighbors = 8;
        static constexpr qlm::Point<int> neighbor_idx[8] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

        for (int y = valid_distance; y < response.height - valid_distance; y++)
        {
            for (int x = valid_distance; x < response.width - valid_distance; x++)
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

                    if (neighbor_x >= valid_distance && neighbor_x < response.width - valid_distance && 
                        neighbor_y >= valid_distance && neighbor_y < response.height - valid_distance)
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
                    corners.emplace_back(Point<int>(x, y), response_value, 1.0f); // use scale to track if the corner is removed or not : 1 -> exists, 0 -> removed 
                }
            }
        }

        // stage 6: radius NMS
        std::sort(corners.begin(), corners.end(), [](const KeyPoint<int>& a, const KeyPoint<int>& b) {
            return a.response > b.response;
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
}
