#include "Multiply.hpp"

namespace qlm
{
    template<ImageFormat frmt, pixel_t T1, pixel_t T2, pixel_t TO>
    Image<frmt, TO> Multiply(const Image<frmt, T1> &in1, const Image<frmt, T2> &in2, const float scale, const OverFlowFlag &over_flow)
    {
        constexpr float min_val = std::numeric_limits<TO>::lowest();
        constexpr float max_val = std::numeric_limits<TO>::max();

        Image<frmt, TO> out(in1.width, in1.height);
        Pixel<frmt, TO> pixel_out;

        for (int y = 0; y < in1.height; y++)
        {
            for (int x = 0; x < in1.width; x++)
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

        return out;
    }

    template Image<ImageFormat::GRAY, uint8_t> Multiply(
        const Image<ImageFormat::GRAY, uint8_t> &,
        const Image<ImageFormat::GRAY, uint8_t> &,
        const float,
        const OverFlowFlag &);

    template Image<ImageFormat::RGB, uint8_t> Multiply(
        const Image<ImageFormat::RGB, uint8_t> &,
        const Image<ImageFormat::RGB, uint8_t> &,
        const float,
        const OverFlowFlag &);

    template Image<ImageFormat::GRAY, float> Multiply(
        const Image<ImageFormat::GRAY, int16_t> &,
        const Image<ImageFormat::GRAY, int16_t> &,
        const float,
        const OverFlowFlag &);
}