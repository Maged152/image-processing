#include "Subtract.hpp"

namespace qlm
{
    template <ImageFormat frmt, pixel_t T_in, pixel_t T_out>
    Image<frmt, T_out> Subtract(const Image<frmt, T_in> &in1, const Image<frmt, T_in> &in2)
    {
        Image<frmt, T_out> out (in1.width, in1.height);

        for (int h = 0; h < in1.height; h++)
        {
            for (int w = 0; w < in1.width; w++)
            {
                const Pixel<frmt, T_out> p1 = in1.GetPixel(w, h);
                const Pixel<frmt, T_out> p2 = in2.GetPixel(w, h);

                const Pixel<frmt, T_out> p_out = p1 - p2;

                out.SetPixel(w, h, p_out);
            }   
        }
        
        return out;
    }

    template Image<ImageFormat::GRAY, int16_t> 
    Subtract(const Image<ImageFormat::GRAY, uint8_t>&, const Image<ImageFormat::GRAY, uint8_t>&);

}