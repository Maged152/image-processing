#include "GoodFeaturesToTrack.hpp"
#include "Sobel.hpp"
#include "BoxFilter.hpp"
#include "Multiply.hpp"

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
        constexpr float neg_inf = std::numeric_limits<float>::lowest();
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

        for (int y = 0; y < in.height; y++)
        {
            for (int x = 0; x < in.width; x++)
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
                    response_pixel.v = (ixx + iyy - std::sqrt((ixx - iyy) * (ixx - iyy) + 4.0f * ixy * ixy)) * 0.5f;
                }

                // find max score value for Shi-Tomasi corner detection
                if (response_pixel.v > max_response_pixel)
                {
                    max_response_pixel = response_pixel.v;
                }

                response.SetPixel(x, y, response_pixel);
            }
        }

        // stage 4: threshold the response and find corners
        const float threshold = max_response_pixel * quality_level;

        for (int y = 0; y < response.height; y++)
        {
            for (int x = 0; x < response.width; x++)
            {
                const float response_value = response.GetPixel(x, y).v;
                if (response_value < threshold)
                {
                    response.SetPixel(x, y, neg_inf);
                }
            }
        }
       

        return std::vector<KeyPoint<int>>();
    }
}
