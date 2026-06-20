#include "Canny.hpp"
#include "Sobel.hpp"
#include <cmath>

template <qlm::pixel_t T>
qlm::Image<qlm::ImageFormat::GRAY, T> Canny(const qlm::Image<qlm::ImageFormat::GRAY, T> &in, 
                                            const int threshold_low, const int threshold_high, 
                                            const int filter_size, const bool l2_gradient, 
                                            const qlm::BorderMode<qlm::ImageFormat::GRAY, T> &border_mode)
{
    // stage 1: Gradient calculation using Sobel operator
    auto gradient = qlm::Sobel(in, filter_size, border_mode);

    // stage 2: Non-maximum suppression
    const auto QuantizeAngle = [](float angle)
    {
        angle = std::round(angle / 45) * 45;
        if (angle < 0)
            angle += 180;
        
        if (angle == 180)
            angle = 0;
                
        return static_cast<int>(angle);
    };

    const auto IsValidIndex = [&](const int idx)
    {
        return idx >= 0 && idx < in.height * in.stride;
    };

    for (int h = 0; h < in.height; h++)
    {
        for (int w = 0; w < in.width; w++)
        {
            // get the current pixel's gradient magnitude and angle
            const int mag = gradient.magnitude.GetPixel(w, h).v;
            // quantize the gradient angle into one of four directions [0, 45, 90, 135] degrees
            const int angle = QuantizeAngle(gradient.angle.GetPixel(w, h).v);

            // determine the two neighboring pixels to compare with based on the quantized angle
            int neighbor0_idx, neighbor1_idx;
            switch (angle)
            {
                case 0:
                    neighbor0_idx = h * in.stride + (w + 1); // right
                    neighbor1_idx = h * in.stride + (w - 1); // left
                    break;
                case 45:
                    neighbor0_idx = (h - 1) * in.stride + (w + 1); // top-right
                    neighbor1_idx = (h + 1) * in.stride + (w - 1); // bottom-left
                    break;
                case 90:
                    neighbor0_idx = (h - 1) * in.stride + w; // top
                    neighbor1_idx = (h + 1) * in.stride + w; // bottom
                    break;
                case 135:
                    neighbor0_idx = (h - 1) * in.stride + (w - 1); // top-left
                    neighbor1_idx = (h + 1) * in.stride + (w + 1); // bottom-right
                    break;
                default:
                    static_assert(false, "Invalid angle quantization");
            }

            // check if the current pixel is a local maximum
            if ((IsValidIndex(neighbor1_idx) && gradient.magnitude.GetPixel(neighbor1_idx).v > mag) ||
                (IsValidIndex(neighbor0_idx) && gradient.magnitude.GetPixel(neighbor0_idx).v > mag))
            {
                gradient.magnitude.SetPixel(w, h, 0); // suppress non-maximum pixel
            }
        }
    }
    return qlm::Image<qlm::ImageFormat::GRAY, T>();
}