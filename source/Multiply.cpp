#include "Multiply.hpp"
#include <cassert>

namespace qlm
{
    template<ImageFormat frmt, pixel_t T1, pixel_t T2, pixel_t TO>
    void Multiply(const Image<frmt, T1> &in1, const Image<frmt, T2> &in2, Image<frmt, TO> &out, const float scale, const OverFlowFlag &over_flow, const Rectangle<int> &roi)
    {
        constexpr float min_val = std::numeric_limits<TO>::lowest();
        constexpr float max_val = std::numeric_limits<TO>::max();

        // validate that the input images have the same dimensions
        assert(in1.width == in2.width && in1.height == in2.height);
        assert(in1.width == out.width && in1.height == out.height);

        const Rectangle<int> valid_roi = roi.ValidROI(in1.width, in1.height);

        const int start_x = valid_roi.top_left.x;
        const int start_y = valid_roi.top_left.y;

        const int end_x = start_x + valid_roi.width;
        const int end_y = start_y + valid_roi.height;

        Pixel<frmt, TO> pixel_out;

        for (int y = start_y; y < end_y; y++)
        {
            for (int x = start_x; x < end_x; x++)
            {
                const auto pixel1 = in1.GetPixel(x, y);
                const auto pixel2 = in2.GetPixel(x, y);

                if constexpr (frmt == ImageFormat::GRAY)
                {
                    float val = static_cast<float>(pixel1.v) * static_cast<float>(pixel2.v) * scale;
                    if (over_flow == OverFlowFlag::SATURATE)
                    {
                        val = std::clamp(val, min_val, max_val);
                    }

                    pixel_out.v = static_cast<TO>(val);
                }
                else if constexpr (frmt == ImageFormat::RGB)
                {
                    float r = static_cast<float>(pixel1.r) * static_cast<float>(pixel2.r) * scale;
                    float g = static_cast<float>(pixel1.g) * static_cast<float>(pixel2.g) * scale;
                    float b = static_cast<float>(pixel1.b) * static_cast<float>(pixel2.b) * scale;

                    if (over_flow == OverFlowFlag::SATURATE)
                    {
                        r = std::clamp(r, min_val, max_val);
                        g = std::clamp(g, min_val, max_val);
                        b = std::clamp(b, min_val, max_val);
                    }

                    pixel_out.r = static_cast<TO>(r);
                    pixel_out.g = static_cast<TO>(g);
                    pixel_out.b = static_cast<TO>(b);
                }

                out.SetPixel(x, y, pixel_out);
            }
        }
    }

    template<ImageFormat frmt, pixel_t T1, pixel_t T2, pixel_t TO>
    Image<frmt, TO> Multiply(const Image<frmt, T1> &in1, const Image<frmt, T2> &in2, const float scale, const OverFlowFlag &over_flow, const Rectangle<int> &roi)
    {
        Image<frmt, TO> out(in1.width, in1.height);
        Multiply(in1, in2, out, scale, over_flow, roi);

        return out;
    }

    template Image<ImageFormat::GRAY, uint8_t> Multiply(
        const Image<ImageFormat::GRAY, uint8_t> &,
        const Image<ImageFormat::GRAY, uint8_t> &,
        const float,
        const OverFlowFlag &,
        const Rectangle<int> &);

    template void Multiply(
        const Image<ImageFormat::GRAY, uint8_t> &,
        const Image<ImageFormat::GRAY, uint8_t> &,
        Image<ImageFormat::GRAY, uint8_t> &,
        const float,
        const OverFlowFlag &,
        const Rectangle<int> &);

    template Image<ImageFormat::RGB, uint8_t> Multiply(
        const Image<ImageFormat::RGB, uint8_t> &,
        const Image<ImageFormat::RGB, uint8_t> &,
        const float,
        const OverFlowFlag &,
        const Rectangle<int> &);

    template void Multiply(
        const Image<ImageFormat::RGB, uint8_t> &,
        const Image<ImageFormat::RGB, uint8_t> &,
        Image<ImageFormat::RGB, uint8_t> &,
        const float,
        const OverFlowFlag &,
        const Rectangle<int> &);

    template Image<ImageFormat::GRAY, float> Multiply(
        const Image<ImageFormat::GRAY, int16_t> &,
        const Image<ImageFormat::GRAY, int16_t> &,
        const float,
        const OverFlowFlag &,
        const Rectangle<int> &);

    template void Multiply(
        const Image<ImageFormat::GRAY, int16_t> &,
        const Image<ImageFormat::GRAY, int16_t> &,
        Image<ImageFormat::GRAY, float> &,
        const float,
        const OverFlowFlag &,
        const Rectangle<int> &);
}