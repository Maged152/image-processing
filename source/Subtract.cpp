#include "Subtract.hpp"
#include <cassert>

namespace qlm
{
    template <ImageFormat frmt, pixel_t T_in, pixel_t T_out>
    void Subtract(const Image<frmt, T_in> &in1, const Image<frmt, T_in> &in2, Image<frmt, T_out> &out, const Rectangle<int> &roi)
    {
        // validate that the input images have the same dimensions
        assert(in1.Width() == in2.Width() && in1.Height() == in2.Height());
        assert(in1.Width() == out.Width() && in1.Height() == out.Height());

        auto [start_x, start_y, end_x, end_y] = ROI(roi, in1.Width(), in1.Height());

        for (int h = start_y; h < end_y; h++)
        {
            for (int w = start_x; w < end_x; w++)
            {
                const Pixel<frmt, T_out> p1 = in1.GetPixel(w, h);
                const Pixel<frmt, T_out> p2 = in2.GetPixel(w, h);

                const Pixel<frmt, T_out> p_out = p1 - p2;

                out.SetPixel(w, h, p_out);
            }   
        }
    }
    template <ImageFormat frmt, pixel_t T_in, pixel_t T_out>
    Image<frmt, T_out> Subtract(const Image<frmt, T_in> &in1, const Image<frmt, T_in> &in2, const Rectangle<int> &roi)
    {
        Image<frmt, T_out> out (in1.Width(), in1.Height());
        Subtract(in1, in2, out, roi);
        
        return out;
    }

    template Image<ImageFormat::GRAY, int16_t> 
    Subtract(const Image<ImageFormat::GRAY, uint8_t>&, const Image<ImageFormat::GRAY, uint8_t>&, const Rectangle<int>&);

    template void
    Subtract(const Image<ImageFormat::GRAY, uint8_t>&, const Image<ImageFormat::GRAY, uint8_t>&, Image<ImageFormat::GRAY, int16_t>&, const Rectangle<int>&);

}