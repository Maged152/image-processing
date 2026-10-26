#pragma once

#include "common/types.hpp"

namespace qlm
{
    template<ImageFormat frmt, pixel_t T>
    Pixel<frmt, T> Min(
        const Image<frmt, T>& in,
        const Rectangle<int>& roi = Rectangle<int>{});

    template<ImageFormat frmt, pixel_t T>
    void Min(
        const Image<frmt, T>& in,
              Pixel<frmt, T>& out,
        const Rectangle<int>& roi = Rectangle<int>{});
}
