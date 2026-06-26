#include "Canny.hpp"
#include "Sobel.hpp"
#include "Magnitude.hpp"
#include <cmath>
#include <limits>
#include <cassert>
#include <vector>

template <qlm::pixel_t T>
qlm::Image<qlm::ImageFormat::GRAY, T> qlm::Canny(const qlm::Image<qlm::ImageFormat::GRAY, T> &in, 
                                            const int threshold_low, const int threshold_high, 
                                            const int filter_size, const bool l2_gradient, 
                                            const qlm::BorderMode<qlm::ImageFormat::GRAY, T> &border_mode)
{
    qlm::Image<qlm::ImageFormat::GRAY, T> out(in.width, in.height);
    constexpr int strong_edge = std::numeric_limits<T>::max();
    constexpr int weak_edge = std::numeric_limits<T>::max() / 2;

    // helper to check coordinate bounds
    const auto IsValidCoord = [&](const qlm::Point<int>& p) 
    {
        return p.x >= 0 && p.x < in.width && p.y >= 0 && p.y < in.height;
    };

    // stage 1: Gradient calculation using Sobel operator
    const auto sobel_x = qlm::SobelX(in, filter_size, border_mode);
    const auto sobel_y = qlm::SobelY(in, filter_size, border_mode);

    auto gradient = qlm::Magnitude(sobel_x, sobel_y, l2_gradient);

    // stage 2: Non-maximum suppression
    const auto CalcQuantizedAngle = [&](const int x, const int y)
    {
        // calculate the angle
        double angle = std::atan2(static_cast<double>(sobel_y.GetPixel(x, y).v), 
                                  static_cast<double>(sobel_x.GetPixel(x, y).v)) * 180.0 / M_PI;

        // quantize the angle to one of four directions [0, 45, 90, 135] degrees
        angle = std::round(angle / 45) * 45;
        if (angle < 0)
            angle += 180;
        
        if (angle == 180)
            angle = 0;
                
        return static_cast<int>(angle);
    };

    std::vector<qlm::Point<int>> stack; // Pre-allocate stack with a reasonable reserve to avoid reallocations
    stack.reserve(static_cast<size_t>(in.width) * in.height / 4);


    for (int y = 0; y < in.height; y++)
    {
        for (int x = 0; x < in.width; x++)
        {
            // get the current pixel's gradient magnitude and angle
            const int32_t mag = gradient.GetPixel(x, y).v;
            const int32_t angle = CalcQuantizedAngle(x, y);

            // determine the two neighboring pixels to compare with based on the quantized angle
            qlm::Point neighbor1, neighbor2;
            switch (angle)
            {
                case 0:
                    neighbor1 = qlm::Point(x - 1, y); // left 
                    neighbor2 = qlm::Point(x + 1, y); // right
                    break;
                case 45:
                    neighbor1 = qlm::Point(x - 1, y - 1); // top-left
                    neighbor2 = qlm::Point(x + 1, y + 1); // bottom-right                    
                    break;
                case 90:
                    neighbor1 = qlm::Point(x, y - 1); // top
                    neighbor2 = qlm::Point(x, y + 1); // bottom
                    break;
                case 135:
                    neighbor1 = qlm::Point(x + 1, y - 1); // top-right
                    neighbor2 = qlm::Point(x - 1, y + 1); // bottom-left
                    break;
                default:
                    assert(false && "Invalid angle quantization");
            }

            // check if the current pixel is a local maximum using coordinate-safe checks
            if ((IsValidCoord(neighbor1) && gradient.GetPixel(neighbor1.x, neighbor1.y).v >= mag) ||
                (IsValidCoord(neighbor2) && gradient.GetPixel(neighbor2.x, neighbor2.y).v > mag))
            {
                out.SetPixel(x, y, 0); // suppress non-maximum pixel
            }
            else if (mag > threshold_high)
            {
                out.SetPixel(x, y, strong_edge); // strong edge
                stack.push_back(qlm::Point(x, y));
            }
            else if (mag > threshold_low)
            {
                out.SetPixel(x, y, weak_edge); // weak edge
            }
            else
            {
                out.SetPixel(x, y, 0); // non-edge
            }
        }
    }

    // stage 3: Double thresholding and edge tracking by hysteresis
    // propagate strong edges through connected weak edges (DFS flood fill)
    while (!stack.empty())
    {
        const auto [cx, cy] = stack.back();
        stack.pop_back();

        for (int dy = -1; dy <= 1; dy++)
        {
            for (int dx = -1; dx <= 1; dx++)
            {
                if (dx == 0 && dy == 0) continue;

                const qlm::Point neighbor(cx + dx, cy + dy);
                // GetPixel returns 0 for out-of-bounds coords, which never equals weak_edge, so no explicit bounds check needed
                if (out.GetPixel(neighbor.x, neighbor.y).v == weak_edge)
                {
                    out.SetPixel(neighbor.x, neighbor.y, strong_edge);
                    stack.push_back(neighbor);
                }   
            }
        }
    }

    // Suppress remaining weak edges in one pass
    for (int y = 0; y < out.height; y++)
    {
        for (int x = 0; x < out.width; x++)
        {
            if (out.GetPixel(x, y).v == weak_edge)
                out.SetPixel(x, y, 0);
        }
    }

    return out;
}

// Explicit instantiation for  uint8_t
template qlm::Image<qlm::ImageFormat::GRAY, uint8_t> qlm::Canny<uint8_t>(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t> &,
                                            const int, const int, const int, const bool,
                                            const qlm::BorderMode<qlm::ImageFormat::GRAY, uint8_t> &);