#include "Min.hpp"

namespace qlm
{
    template <ImageFormat frmt, pixel_t T>
    void Min(const Image<frmt, T>& in, Pixel<frmt, T>& out, const Rectangle<int>& roi)
    {
        auto [start_x, start_y, end_x, end_y] = ROI(roi, in.Width(), in.Height());

        // initialize the result with the first pixel of the ROI
        Pixel<frmt, T> result = in.GetPixel(start_x, start_y);

        for (int y = start_y; y < end_y; y++)
        {
            for (int x = start_x; x < end_x; x++)
            {
                const Pixel<frmt, T> p = in.GetPixel(x, y);
                result.v = std::min(result.v, p.v);
            }
        }

        out = result;
    }

    template <ImageFormat frmt, pixel_t T>
    Pixel<frmt, T> Min(const Image<frmt, T>& in, const Rectangle<int>& roi)
    {
        Pixel<frmt, T> out;
        Min(in, out, roi);

        return out;
    }

    template Pixel<ImageFormat::GRAY, uint8_t> Min(
        const Image<ImageFormat::GRAY, uint8_t>&,
        const Rectangle<int>&);

    template void Min(
        const Image<ImageFormat::GRAY, uint8_t>&,
        Pixel<ImageFormat::GRAY, uint8_t>&,
        const Rectangle<int>&);

    template Pixel<ImageFormat::GRAY, float> Min(
        const Image<ImageFormat::GRAY, float>&,
        const Rectangle<int>&);

    template void Min(
        const Image<ImageFormat::GRAY, float>&,
        Pixel<ImageFormat::GRAY, float>&,
        const Rectangle<int>&);
}
