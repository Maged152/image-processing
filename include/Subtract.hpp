#pragma once

#include "common/types.hpp"

namespace qlm
{
    template<ImageFormat frmt, pixel_t T_in, pixel_t T_out>
    Image<frmt, T_out> Subtract(
        const Image<frmt, T_in> &in1,
        const Image<frmt, T_in> &in2,
        const Rectangle<int> &roi = Rectangle<int>{});

    template<ImageFormat frmt, pixel_t T_in, pixel_t T_out>
    void Subtract(
        const Image<frmt, T_in> &in1,
        const Image<frmt, T_in> &in2,
              Image<frmt, T_out> &out,
        const Rectangle<int> &roi = Rectangle<int>{});
}