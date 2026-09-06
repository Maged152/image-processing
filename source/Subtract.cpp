#include "Subtract.hpp"
#include <cassert>

namespace qlm
{
    template <ImageFormat frmt, pixel_t T_in, pixel_t T_out>
    void Subtract(const Image<frmt, T_in> &in1, const Image<frmt, T_in> &in2, Image<frmt, T_out> &out, const Rectangle<int> &roi)
    {
        // validate that the input images have the same dimensions
        assert(in1.width == in2.width && in1.height == in2.height);
        assert(in1.width == out.width && in1.height == out.height);

        const Rectangle<int> valid_roi = roi.ValidROI(in1.width, in1.height);

        const int start_w = valid_roi.top_left.x;
        const int start_h = valid_roi.top_left.y;

        const int end_w = start_w + valid_roi.width;
        const int end_h = start_h + valid_roi.height;

        for (int h = start_h; h < end_h; h++)
        {
            for (int w = start_w; w < end_w; w++)
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
        Image<frmt, T_out> out (in1.width, in1.height);
        Subtract(in1, in2, out, roi);
        
        return out;
    }

    template Image<ImageFormat::GRAY, int16_t> 
    Subtract(const Image<ImageFormat::GRAY, uint8_t>&, const Image<ImageFormat::GRAY, uint8_t>&, const Rectangle<int>&);

    template void
    Subtract(const Image<ImageFormat::GRAY, uint8_t>&, const Image<ImageFormat::GRAY, uint8_t>&, Image<ImageFormat::GRAY, int16_t>&, const Rectangle<int>&);

}